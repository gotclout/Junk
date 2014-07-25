#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/resource.h>

#define MAX_LEN 256
#ifndef ARG_MAX
  #define M_ARG_MAX sysconf(_SC_ARG_MAX)
  static size_t _ARG_MAX = M_ARG_MAX;
  #define ARG_MAX _ARG_MAX
#endif

using namespace std;

/**
 * Contains data associated with a command to be executed
 */
struct CmdData
{
  char* args[MAX_LEN], *pth, *inf, *outf; //args, path to exec io redirect
  bool amp; //execute no wait
  int argc , plen; //num args

  /**
   * Default Constructor
   */
  CmdData() {plen = 2*(MAX_LEN)*sizeof(char); pth = 0; init();};

  /**
   * Initializes CmdData members
   */
  bool init()
  {
    if(pth) { del(); } memset((pth = (char*)malloc(plen)), 0, plen);
    memset(args, 0, sizeof(char*)*MAX_LEN);
    return (inf = outf = (char*) !(&(amp = (bool)(argc = 0)))) == 0;
  };

  /**
   * Retrieves the argument at the ith index
   */
  char* at(int i = 0) { return *args && i < argc ? args[i] : 0; };

  /**
   * Retrieves the jth character of the ith argument
   */
  char argat(int i = 0, int j = 0) { return at(i) ? *(at(i) + j) : '\0'; };

  /**
   * Free allocated data
   */
  void del()
  {
    if(pth) free(pth); pth =0 ;
    for(int i = 0; i < argc; ++i) if(at(i))free(at(i));
  };
};

/**
 * Execute forks a process to execute the specified command
 *
 * @param char** args, the command and arguments
 * @param char* path, the full path to the executable
 * @param char** inn, is the redirection input param
 * @param char** outt, is the redirection output param
 * @param bool amp, indicates whether or not the main proc should wait
 *  for the child proc or return immediately
 */
bool execute(CmdData & cmd)
{
  int ifid, ofid;
  pid_t pid = fork(); //create child
  if(pid < 0) { cerr << "ERROR: could not fork subprocess\n"; return 0; }
  else if(pid == 0)             //child process
  {
    if(cmd.inf) ifid=open(cmd.inf,O_RDONLY|O_NONBLOCK,00222);
    if(cmd.outf )ofid=open(cmd.outf,O_WRONLY|O_RDONLY|O_CREAT,00666);
    execv(cmd.pth, cmd.args);   //execute command
    if(cmd.inf) close(ifid);    //end redirection
    if(cmd.outf) close(ofid);
    perror(*cmd.args);
    exit(0);
  }
  else if(!cmd.amp) waitpid(pid, 0, 0);
  else usleep(50000);
  return 1;
}

/**
 * canexec determines whether or not the specified file is executable
 *
 * @param const char* pPath is the file to be executed
 * @param bool errWarn indicates whether or not errors should be issued
 * @return int8_t 1 if ok, 0 if not executable -1 if DNE
 */
int8_t canexec(const char* pPath, char err[], int rv = -1)
{
  if(!access(pPath, F_OK) && !++rv)         //exists
  {
    if(!access(pPath, X_OK)) rv++; //executable
    else strcat(strncpy(err, pPath, strlen(pPath)), " is not executable\n");
  }
  else strcat(strcpy(err, 1+strrchr(pPath, '/')), " does not exist\n");
  return rv;
}

/**
 * Locates the specified command and verifies that it is executable
 *
 * @param char** args is the string array of command arguments and the
 *  first entry is the command to be executed
 * @return char* is a path to the command to be executed (may be null)
 */
bool get_cmd_pth(CmdData & cmd)
{
  char err[MAX_LEN];
  int errno     = -1, i = 1;
  char* pwdenv  = getenv("PWD"), *pthenv = 0;
  size_t pwdln  = strlen(pwdenv), pthenvln, cpyln;
  if(cmd.argat() != '.' && cmd.argat() != '/' && (pthenv = getenv("PATH"))) //cmd
  {
    char pth[(pthenvln = strlen(pthenv))+1];
    strncpy((char*)memset(pth, 0, pthenvln + 1), pthenv, pthenvln);
    for(char* p = strtok(pth, ":"); p && errno == -1; p = strtok(0, ":"))
    {
      memset(cmd.pth, 0, cmd.plen);
      if(*p == '.')strcat(strcat(strncpy(cmd.pth, pwdenv, pwdln), "/"), cmd.at());
      else strcat(strcat(strncpy(cmd.pth, p, strlen(p)), "/"), cmd.at());
      errno = canexec(cmd.pth, (char*)memset(err, 0, MAX_LEN));
    }
  }
  else // ./cmd else ../ else err
  {
    if(cmd.argat() == '/'){ strcat(cmd.pth, cmd.at()); i = 0;}
    else if(cmd.argat() == '.' && cmd.argat(0,i) == '/') cpyln = pwdln;
    else if(cmd.argat(0,i++) == '.')cpyln = pwdln-strlen(strrchr(pwdenv, '/'));
    else {cerr << "ERROR: Invalid command input\n"; return 0;}
    if(i)strcat(strncpy(cmd.pth, pwdenv, cpyln), cmd.at()+i);
    errno = canexec(cmd.pth, (char*)memset(err, 0, MAX_LEN));
  }
  return errno == 1 || !(cerr << "ERROR: " << err);
}

/**
 * Counts and parses the command line into args
 *
 * @param char** args, the string array of parsed arguments
 * @param int num_args, the num arguments in args
 * @return bool true if success, false otherwise
 */
bool get_args(CmdData & d, char* buf = 0)//char* args[], int & argc)
{
  cout << "\n>> "; //prompt
  size_t ap, len,  pos = getline(&buf, &ARG_MAX, stdin), cpos = pos;//read
  if(!(d.argc = 0) && pos > 0)
  {
    while(pos-- && !(ap = 0))
    {
      while(buf && !isspace(*buf++)){ --pos; ++ap; }
      d.args[d.argc] = (char*)malloc((len = 1 + sizeof(char) * ap++));
      strncpy((char*)memset(d.args[d.argc++], 0, len), buf - ap, ap - 1);
    }
    free((buf - cpos));
  }
  return d.argc > 0;
}

/**
 * Read a list of commands and arguments from the command line
 * stores them and executes as a generic shell using child procs
 * via fork execv and waitpid
 */
int main(int argc, char** argv)
{
  for(CmdData cmd; get_args(cmd) && strcmp(cmd.at(),"exit"); cmd.init())
  {
    for(int i = --argc; i < cmd.argc; ++i) //check for & < > flags
    {
      if(cmd.argat(i,0) == '<') cmd.inf = cmd.at(++i);
      else if(cmd.argat(i,0) == '>') cmd.outf = cmd.at(++i);
      else if(cmd.argat(i,0) == '&') cmd.amp = true;
      else if(!cmd.inf && !cmd.outf && !cmd.amp) ++argc;
      else if(cmd.at(argc))*cmd.args[argc] = '\0';
    }
    if(get_cmd_pth(cmd)) execute(cmd);
  }

  return 0;
}
