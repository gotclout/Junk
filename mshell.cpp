#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <sstream>
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
  char* args[MAX_LEN], *path, *inf, *outf; //args, path to exec io redirect
  bool amp; //execute no wait
  int argc; //num args

  /**
   * Default Constructor
   */
  CmdData()
  {
    memset(args, 0, sizeof(char*)*MAX_LEN);
    argc = 0;
    amp = 0; 
    path = inf = outf = 0;
  };

  /**
   * Retrieves the argument at the ith index
   */
  char* at(int i = 0) { return *args && i < argc ? args[i] : 0; };

  /**
   * Retrieves the jth character of the ith argument
   */
  char argat(int i = 0, int j = 0) { return at(i) ? *at(i) + j : '\0'; };
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
  bool ret = true;
  int fid, pid = fork();       //create child
  if(pid < 0) ret = false;
  if(!ret) cerr << "ERROR: could not fork subprocess\n";
  else if(pid == 0)            //child process
  {
    if(cmd.inf && !close(0)) fid=open(cmd.inf,O_RDONLY|O_NONBLOCK,00222);
    if(cmd.outf && !close(1))fid=open(cmd.outf,O_WRONLY|O_RDONLY|O_CREAT,00666);
    execv(cmd.path, cmd.args); //execute command
    if(cmd.inf) close(0);      //end redirection
    if(cmd.outf) close(1);
    perror(*cmd.args);
    exit(0);
  }
  else if(!cmd.amp) waitpid(pid, NULL, 0);
  else usleep(50000);
  return ret;
}

/**
 * canexec determines whether or not the specified file is executable
 *
 * @param const char* pPath is the file to be executed
 * @param bool errWarn indicates whether or not errors should be issued
 * @return int8_t 1 if ok, 0 if not executable -1 if DNE
 */
int8_t canexec(const char* pPath, char err[])
{
  int8_t rv = -1;
  memset(err, 0, MAX_LEN);
  if(!access(pPath, F_OK))         //exists
  {
    rv++;
    if(!access(pPath, X_OK)) rv++; //executable
    else strcat(strncpy(err, pPath, strlen(pPath)), " is not executable\n");
  }
  else strcat(strncpy(err, pPath, strlen(pPath)), " does not exist\n");
  return rv;
}

/**
 * Locates the specified command and verifies that it is executable
 *
 * @param char** args is the string array of command arguments and the
 *  first entry is the command to be executed
 * @return char* is a path to the command to be executed (may be null)
 */
bool get_cmd_path(char* args, char* & cmdpth)
{
  char errstr[MAX_LEN];
  int errno     = -1,
      i         = 1;
  char* pwdenv  = getenv("PWD");
  size_t pwdlen = strlen(pwdenv), pthenvlen, cpylen;

  cmdpth        = (char*) malloc(2*(MAX_LEN)*sizeof(char)); //path vairable
  memset(cmdpth, 0, 2*MAX_LEN*sizeof(char));
  if(*args != '.' && *args != '/')         //    cmd
  {
    char* pthenv = getenv("PATH");
    char path[( pthenvlen = strlen(pthenv))+1];
    memset(path, 0, MAX_LEN);
    strncpy(path, pthenv, pthenvlen);
    for(char* p = strtok(path, ":"); p && errno == -1; p = strtok(0, ":"))
    {
      if(*p == '.') strcat(strcat(strncpy(cmdpth, pwdenv, pwdlen), "/"), args);
      else strcat(strcat(strncpy(cmdpth, p, strlen(p)), "/"), args);
      errno = canexec(cmdpth, errstr);
      if(errno == -1) memset(cmdpth, 0, 2*MAX_LEN*sizeof(char));
    }
  }
  else
  {
    if(*args == '/') strcat(cmdpth, args);  //   /cmd
    else if(*args == '.' && *args+i == '/') cpylen = pwdlen;
    else if(*args+i++ == '.') cpylen = pwdlen - strlen(strrchr(pwdenv, '/'));
    else ++i;
    if(i < 3) strcat(strncpy(cmdpth, pwdenv, cpylen), args+i);
    errno = canexec(cmdpth, errstr);
  }
  if(errno < 1) cerr << "ERROR: " << errstr << '\n';
  return errno == 1;
}

/**
 * Counts and parses the command line into args
 *
 * @param char** args, the string array of parsed arguments
 * @param int num_args, the num arguments in args
 * @return bool true if success, false otherwise
 */
bool parse_args(char* args[], int & argc)
{
  bool rv = true;
  char* buf = 0, *bptr = 0; //save buf so that it can be deallocated
  cout << "\n>> ";          //prompt
  size_t ap, len, pos = getline(&buf, &ARG_MAX, stdin); //read command line
  if(pos < 1) rv = false;
  else
  {
    bptr = &(*buf);
    while(pos)
    {
      --pos;
      ap = 0;
      while(buf && !isspace(*buf++))
      {
        --pos;
        ++ap;
      }
      len = 1 + sizeof(char) * ap++;
      args[argc] = (char*)malloc(len);
      memset(args[argc], 0, len);
      strncpy(args[argc++], buf - ap, ap - 1);
    }
    free(bptr);
  }
  return rv;
}

/**
 * Read a list of commands and arguments from the command line
 * stores them and executes as a generic shell using child procs
 * via fork execv and waitpid
 */
int main(int argc, char** argv)
{
  int i = 0, pos;
  CmdData cmd, *cmdp = &cmd;
  while(parse_args(cmd.args, cmd.argc) && strcmp(cmdp->args[0], "exit") != 0)
  {
    for(i = 0, pos = 0; i < cmd.argc; ++i) //check for & < > flags
    {
      if(cmd.argat(i,0) == '<') cmd.inf = cmd.at(++i);
      else if(cmd.argat(i,0) == '>') cmd.outf = cmd.at(++i);
      else if(cmd.argat(i,0) == '&') cmd.amp = true;
      else if(!cmd.inf && !cmd.outf && !cmd.amp)++pos;
    }
    if(pos && cmd.at(pos)) cmd.args[pos] = 0;
    if(get_cmd_path(*cmd.args, cmd.path)) execute(cmd);
    free(cmd.path);
    for(i = 0; i < cmd.argc; ++i) if(cmd.at(i)) free(cmd.at(i));  //free args
  }

  return 0;
}

