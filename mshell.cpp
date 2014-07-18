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
 * Execute forks a process to execute the specified command
 *
 * @param char** args, the command and arguments
 * @param char* path, the full path to the executable
 * @param char** inn, is the redirection input param
 * @param char** outt, is the redirection output param
 * @param bool amp, indicates whether or not the main proc should wait
 *  for the child proc or return immediately
 */
bool execute(char** args, char* path, char* inf, char* outf, bool amp)
{
  bool ret = true;
  int fid, pid = fork();  //create child
  if(pid < 0)
  {
    cerr << "ERROR: could not fork subprocess\n";
    ret = false;          //exception
  }
  else if(pid == 0)       //child process
  {
    if(inf && !close(0))  fid = open(inf, O_RDONLY|O_NONBLOCK, 00222);
    if(outf && !close(1)) fid = open(outf, O_WRONLY|O_RDONLY|O_CREAT, 00666);
    execv(path, args); //execute command
    if(inf) close(0);               //end redirection
    if(outf) close(1);
    perror(*args);
    exit(0);
  }
  else if(!amp) waitpid(pid, NULL, 0);
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
int8_t canexec(const char* pPath, bool errWarn = true)
{
  int8_t rv = -1;
  if(!access(pPath, F_OK))              //exists
  {
    rv++;
    if(!access(pPath, X_OK)) rv++; //executable
    else if(errWarn) cerr << "ERROR: " << pPath << " is not executable\n";
  }
  else if(errWarn) cerr << "ERROR: " << pPath << " does not exist\n";
  return rv;
}

/**
 * Locates the specified command and verifies that it is executable
 *
 * @param char** args is the string array of command arguments and the
 *  first entry is the command to be executed
 * @return char* is a path to the command to be executed (may be null)
 */
bool get_cmd_path(char* args, char* & s_path)
{
  int errno     = -1;
  char *pathenv = getenv("PATH"),
       *pwdenv  = getenv("PWD");
  size_t plen   = strlen(pathenv), pwdlen = strlen(pwdenv);
  s_path  = (char*) malloc(2*(MAX_LEN)*sizeof(char)); //path vairable
  memset(s_path, 0, 2*MAX_LEN*sizeof(char));
  if(*args != '.' && *args != '/')         //    cmd
  {
    char path[plen + 1];
    strncpy(path, pathenv, plen);
    for(char* p = strtok(path, ":"); p && errno == -1; p = strtok(0, ":"))
    {
      if(!p) continue;
      if(*p == '.') strcat(strcat(strncpy(s_path, pwdenv, pwdlen), "/"), args);
      else strcat(strcat(strncpy(s_path, p, strlen(p)), "/"), args);
      errno = canexec(s_path, false);
      if(errno < 1) memset(s_path, 0, 2*MAX_LEN*sizeof(char));
    }
    if(errno == -1) cerr << "ERROR: " << args << " does not exist\n";
    else if(errno == 0) cerr << "ERROR: " << s_path << " is not executable\n";
  }
  else
  {
    if(*args == '/') strcat(s_path, args);  //   /cmd
    else if(*args == '.' && *args+1 == '/')
    {
        strncpy(s_path, pwdenv, pwdlen);
        strcat(s_path, args+1);             //  ./cmd
    }
    else if(*args == '.' && *args+1 == '.')
    {
      strncpy(s_path, pwdenv, pwdlen - strlen(strrchr(pwdenv, '/')));
      strcat(s_path, args+2);               // ../cmd
    }
    errno = canexec(s_path);
  }
  return errno == 1;
}

/**
 * counts and parses the command line into args
 *
 * @param char** args, the string array of parsed arguments
 * @param int num_args, the num arguments in args
 * @return bool true if success, false otherwise
 */
bool parse_args(char** args, int & argc)
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
  char* args[MAX_LEN]      ={0};         //parsed command list
  char* path               = NULL;       //parsed path
  char* inf                = NULL;       //redirection input file name
  char* outf               = NULL;       //redirection output file name
  bool amp = 0;                          //& flag
  int i = 0, pos;

  argc = 0;
  parse_args(args, argc); //parse command line
  while((strcmp(args[0], "exit"))) //loop until exit
  {
    for(i = 0, pos = 0; i < argc; ++i) //check for ampersand and redirection
    {
      if(*args[i] == '<') inf = args[++i];
      else if(*args[i] == '>') outf = args[++i];
      else if(*args[i] == '&')amp = true;
      else if(!inf && !outf && !amp)++pos;
    }
    if(args[pos]) args[pos] = 0;
    if(get_cmd_path(*args, path)) execute(args, path, inf, outf, amp);
    free(path);
    path = 0;
    for(i = 0; i < argc; ++i) if(args[i]) free(args[i]); //free args
    argc = 0;                                            //reset
    inf = outf = NULL;
    parse_args(args, argc);                              //repeat
  }
  for(i = 0; i < argc; ++i) if(args[i]) free(args[i]);

  return 0;
}

