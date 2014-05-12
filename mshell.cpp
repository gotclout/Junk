#include <unistd.h>
#include <iostream>
#include <wait.h>
#include <string.h>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <errno.h>

using namespace std;

static size_t max_line = 256;  //max cmd + arg len

/**
 * Returns the smallest value between to values
 *
 * @param size_t lhs, is the value to be compared on the left of <
 * @param size_t rhs, is the value to be compared on the riught of <
 * @return size_t, the smallest value after comparing lhs < rhs
 */
inline static size_t minf(const size_t lhs, const size_t rhs)
{
  return lhs < rhs ? rhs : lhs;
}

/**
 * Executes the specified command
 *
 * @param char** args, the command and arguments
 * @param char* path, the full path to the executable
 * @param char** inn, is the redirection input param
 * @param char** outt, is the redirection output param
 * @param bool amp, indicates whether or not the main proc should wait
 *  for the childe proc or return immediately
 */
bool execute(char** args, char* path, char* inn, char* outt, bool amp)
{
  int status, fid,
      pid = fork(); //create child;

  if(pid < 0)       //exception
  {
    perror("fork");
    exit(1);
  }
  else if(pid == 0) //child process
  {
    if(inn)         //check for redirection
    {
      close(0);
      fid = open((const char*)inn, O_RDONLY | O_NONBLOCK, 00222);
    }
    if(outt)
    {
      close(1);
      fid = open((const char*)outt, O_WRONLY | O_RDONLY | O_CREAT, 00666);
    }
    int err = execv((const char*)path, args);  //execute command
    if(err == -1)
      perror(strerror(errno));
    if(inn) close(0);
    if(outt)close(1);
    exit(0);
  }
  else if(!amp)
    waitpid(pid, NULL, 0);
}

/**
 * Locates the specified command and verifies that it is executable
 *
 * @param char** args is the string array of command arguments and the
 *  first entry is the command to be executed
 */
char* get_path(char** args)
{
  char *pathenv = getenv("PATH"),
       *s_path  = (char*) malloc(max_line*2),
       *ret     = 0,
       *p       = 0;
  size_t plen   = strlen(pathenv);
  char path[plen+3];
  memset(path, 0, plen+3);
  strcat(path, ".:");
  strncpy(path+2, pathenv, plen);
  p = strtok(path, ":");
  while(p && !ret)
  {
    plen = strlen(p);
    memset(s_path, 0, max_line*2*sizeof(char));
    if(*args[0] == '.')
    {
      (*args) += 1;
      strncpy(s_path, getenv("PWD"), strlen(getenv("PWD")));  // ./cmd
      if(*args[0] == '.') (*args) += 1;                       // ../cmd
    }
    else                                                      // /cmd or cmd
    {
      if(*p == '.') strncpy(s_path, get_current_dir_name(), minf(plen, max_line*2));
      else strncpy(s_path, p, minf(plen, max_line*2));
      if(*args[0] != '/') strcat(s_path, "/");
    }
    strcat(s_path, args[0]);
    if(!access(s_path, F_OK))
    {
      if(!access(s_path, X_OK)) ret = s_path;
      else
        cerr << "ERROR: " << s_path << " is not executable" << endl;
    }
    else
      cerr << "ERROR: " << s_path << " not found" << endl;
    p = strtok(NULL, ":");
  }
  if(!ret) free(s_path);

  return ret;
}

/**
 * parses the command line into args
 *
 * @param char* buf, the command line entry
 * @param char** args, the string array of parsed arguments
 * @param int num_args, the num arguments in args
 */
void parse_args(char* buf, char** args, int & num_args)
{
  size_t pos = strlen(buf), ap;
  while(pos)
  {
    pos--;
    ap = 0;
    while(!isspace(*buf++))
    {
      pos--;
      ap++;
    }
    *(buf - 1) = NULL;
    *args++ = buf - ap - 1;
    num_args++;
  }
}

/**
 * Read a list of commands and arguments from the command line
 * stores them and executes as a generic shell using child procs
 * via fork execv and waitpid
 */
int main(int argc, char** argv)
{
  int num_args = 0,               //num args
      i        = 0;               //loop var
  char *args[max_line],           //parsed command list
                *PATH = NULL,     //parsed path
                *inf  = NULL,     //redirection input file name
                *outf = NULL,     //redirection output file name
        *list_command = NULL;     //entire command list

  memset(args, 0, sizeof(char*)*max_line);
  cout << ">> ";                              //prompt
  getline(&list_command, &max_line, stdin);   //read command line
  parse_args(list_command, args, num_args);   //parse command line

  //loop until exit
  while((strcmp(args[0], "exit")))
  {
    int fflag = num_args;                     //num of args til first flag
    bool amp  = 0;                            //wait &
    for(int i = 1; i < num_args; ++i)         //check for & < >
    {
      if(*args[i]== '>') outf = args[i+1];
      else if(*args[i]== '>') inf = args[i+1];
      else if(*args[i]== '&') amp = true;
      if(inf || outf || amp && fflag == num_args) fflag = i;
    }
    args[fflag] = 0;
    PATH = get_path(args);
    if(PATH)
    {
      execute(args, PATH, inf, outf, amp);
      free(PATH);
    }
    else
      cerr << "ERROR: Path to " << args[0] << " not found\n";

    num_args = fflag = 0;
    inf = outf = NULL;
    memset(args, 0, max_line*sizeof(char*));  //reset args to null
    cout << "\n>> ";                          //restart
    getline(&list_command, &max_line, stdin);
    cout << "\n";
    parse_args(list_command, args, num_args);
  }

  return 0;
}

