#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/resource.h>

#include <iostream>

#define MAX_ARGS 256
#define MAX_PATH 1024
#define MAX_LEN  2048
#ifndef ARG_MAX
  #define M_ARG_MAX sysconf(_SC_ARG_MAX)
  static size_t _ARG_MAX = M_ARG_MAX;
  #define ARG_MAX _ARG_MAX
#endif

using namespace std;
/******************************************************************************
 *                        Globals & Prototypes
 *****************************************************************************/
bool parse(char** args, int & argc);
bool execute(char** args, char* path, char* ifile, char* ofile, bool amp = 0);
char* findcmd(char** args);

/**
 *
 */
int main(int argc, char** argv)
{
  char* args[MAX_ARGS]={0};               //parsed command list
  char* path = NULL;                      //parsed path
  char* ifile = NULL;                     //redirection input file name
  char* ofile = NULL;                     //redirection output file name
  char* prog_args[MAX_ARGS] = {0};        //arguments to executable
  bool amp = 0;                           //& flag
  int i, j;

  argc = 0;
  parse(args, argc); //parse command line

  while((strcmp(args[0], "exit"))) //loop until exit
  {
    for(i = 1; i < argc; ++i) //check for ampersand and redirection
    {
      if(*args[i]== '>') ofile = args[i+1];
      else if(*args[i]== '>') ifile = args[i+1];
      else if(*args[i]== '&') amp = true;
    }
    for(i = 0, j = 0; i < argc; ++i)
    {
      if(*args[i] == '&') continue;
      prog_args[j++] = args[i];
    }
    path = findcmd(args); //TODO: pass in path
    if(path)
    {
      execute(prog_args, path, ifile, ofile, amp);
      free(path);
    }
    //clean up
    if(argc > 0)
    {
      for(i = 0; i < argc; ++i)
      {
        if(args && args[i]) free(args[i]);
        args[i] = 0;
      }
    }
    //reset counts, flags, and prog args then prompt and repeat
    argc = 0;
    ifile = ofile = NULL;
    memset(prog_args, 0, MAX_ARGS * sizeof(char*));
    parse(args, argc);
  }
  //clean up
  if(argc > 0)
  {
    for(i = 0; i < argc; ++i)
    {
      if(args && args[i]) free(args[i]);
      args[i] = 0;
    }
  }
  return 0;
}

/**
 *
 */
bool parse(char** args, int & argc)
{
  bool rv = true;
  char* buf = 0;
  cout << "\n>> ";                  //prompt
  getline(&buf, &ARG_MAX, stdin);   //read command line
  size_t pos = strlen(buf), ap;
  while(pos)
  {
    --pos;
    ap = 0;
    while(!isspace(*buf++))
    {
      --pos;
      ++ap;
    }
    *(buf - 1) = NULL;
    *args++ = buf - ap - 1;
    ++argc;
  }
  if(argc < 1) rv = false;
  return rv;
}

/**
 *
 */
bool execute(char** args, char* path, char* ifile, char* ofile, bool amp)
{
  bool ret = true;
  int fid, pid = fork();  //create child
  if(pid < 0)
  {
    cerr << "ERROR: could not fork subprocess\n"; //exception
    ret = false;
  }
  else if (pid == 0)//child process
  {
    //check for redirection, may need to close first
    if(ifile) fid = open((const char*)ifile, O_RDONLY|O_NONBLOCK, 00222);
    if(ofile) fid = open((const char*)ofile, O_WRONLY|O_RDONLY|O_CREAT, 00666);
    execv((const char*)path, args); //execute command
    if(ifile) close(0); //end redirection
    if(ofile) close(1);
    perror(*args);
    exit(0);
  }
  else if(!amp) waitpid(pid, NULL, 0);
  else usleep(50000);
  return ret;
}

/**
 *
 */
bool canexec(const char* pPath)
{
  bool rv = false;
  if(!access(pPath, F_OK))              //exists
  {
    if(!access(pPath, X_OK)) rv = true; //executable
    else cerr << "ERROR: " << pPath << " is not executable\n";
  }
  else cerr << "ERROR: " << pPath << " does not exist\n";
  return rv;
}

/**
 *
 */
char* findcmd(char** args)
{
  int i = 0;                               //index
  char* s_path = (char*) malloc (MAX_LEN); //path vairable
  char* pp [MAX_PATH];                     //array of paths
  char path[MAX_PATH];                     //unparsed line from get PATH

  memset(path, 0, MAX_PATH * sizeof(char));
  strcpy(path, getenv("PATH"));
  char* p = strtok(path, ":"), *ret = 0;
  pp[i] = p;
  while(p && !ret) //while paths to search and path not found
  {
    memset(s_path, 0, MAX_LEN * sizeof(char));
    if(*args[0] != '.' && *args[0] != '/') //command form of filename
    {
      strcpy(s_path, pp[i]);
      if(s_path[0] == '.') strcpy(s_path, get_current_dir_name());
      strcat(strcat(s_path, "/"), args[0]);
      if(canexec(s_path)) ret = s_path;
    }
    else if(*args[0] =='/')                //command form of /filename
    {
      if(canexec(args[0]))
      {
        strcat(s_path, args[0]);
        ret = s_path;
      }
      else p = 0;
    }
    else if(args[0][1] == '/')             //command form of ./filename
    {
      args[0] = &args[0][2];
      strcpy(s_path, getenv("PWD"));
      strcat(strcat(s_path, "/"), args[0]);
      if(canexec(s_path)) ret = s_path;
      else p = 0;
    }
    else                                   //command form of ../filename
    {
      args[0] = &args[0][3];
      strcpy(s_path, getenv("PWD"));
      int j = strlen(s_path);              //decrement counter
      while(j > 0 && s_path[j] != '/') s_path[--j] = NULL;
      strcat(s_path, args[0]);
      if(canexec(s_path)) ret = s_path;
      else p = 0;
    }
    if(!ret)
    {
      p = strtok(NULL, ":");
      pp[++i] = p;
    }
  }
  if(!ret) free(s_path);
  return ret;
}
