

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

using namespace std;

/****************************************FUNCTION PROTOTYPES*****************************************************/

void get_args(char** prog_args, int & fflag, char** args);
void parse(char* buf, char** args, int & num_args);
void execute(char** args, char* path, char* inn, char* outt);
void execute_a(char** args, char* path, char* inn, char* outt);
char* f_path(char** args);
bool redirect_wait(char*& inn, char*& outt, int num_args, char** args, int & first_flag);

////////////////////////////////////////////////////////////////////////
//
//  Project 1 for CS 3204 Spring 2003
//
//  Programmer:    Robert Foster
//  OS:            Mandrake Linux 9.0
//  email:       rfoster@vt.edu
//  System:        Pentium IV 1500, 256 MB Memory
//  Compiler:      g++
//  Last modified: May 21, 2003
//
//  Purpose
//  This program reads a list of commands and arguments from the 
//  command line stores them and acts as a normal shell creating
//  child processes using fork and exec(multi-threading)
//  
//
//  The program then writes a summary of its findings to an output
//  file.
//
int main()
{

  //Declarations
  char* list_command = NULL;              //entire command list
  int num_args =0;                  //num args
  char* args[256]={0};                //parsed command list
  size_t size = 101;                  //read size
  char* PATH = NULL;                  //parsed path
  char* inn  = NULL;                  //redirection input file name
  char* outt = NULL;                  //redirection output file name
  int fflag = 0;                    //num of args til first flag
  char*prog_args[256] ={0};              //arguments to executable
  bool amp;                      //& flag

  cout << ">> ";                    //prompt
  getline(&list_command, &size, stdin);        //read command line
  parse(list_command, args, num_args);        //parse command line

  char EXIT[] = {'e','x','i','t','\0'};        //exit 
  char AMP[] = {'&','\0'};              //flag

  //loop until exit
  while( (strcmp(args[0], EXIT)))
  {
    //check for ampersand and redirection
    amp = redirect_wait(inn, outt, num_args, args, fflag);

    //execute and wait
    if(amp == 1)
    {
      get_args(prog_args, fflag, args);
      PATH = f_path(args);

      if(PATH != NULL)
        execute_a(prog_args, PATH, inn, outt);
    }

    //execute no wait
    else
    {
      get_args(prog_args, fflag, args);
      PATH = f_path(args);

      if(PATH != NULL)
        execute(prog_args, PATH, inn, outt);
    }

    num_args =0;
    fflag = 0;
    inn = NULL;
    outt = NULL;
    //reset prog args to null
    for(int f = 0; f < 256; f++)
      prog_args[f] = NULL;

    //restart
    cout << ">> ";
    getline(&list_command, &size, stdin);
    parse(list_command, args, num_args);

  }


  return 0;
}

//////////////////////////////////////////////////////////////// 
// get_args parse args into array seperate off command and flags
//
// Parameters:
//   (char** prog_args, int & fflag, char** args)
//
// Pre:       command input is parsed
//            
//            
//
// Post:      command executes
//             
//             
//
// Returns:   void
//
// Called by: main
// Calls:     none
//
void get_args(char** prog_args, int & fflag, char** args)
{
  int j = 0; //index
  //read in arguments until first flag
  if(fflag > 1)
  {
    for(int i = 1; i< fflag; i++)
    {
      if(*args[i] != '&')
      {
        prog_args[j] = args[i];
        j++;
      }
    }
  }

}

//////////////////////////////////////////////////////////////// 
// redirect_wait check for redirection and & flag
//
// Parameters:
//   (char*& inn, char*& outt, int num_args, char** args, int & first_flag)
//
// Pre:       flags are passed in command line
//            
//            
//
// Post:      out put and input file names retrieved flagged &
//             
//             
//
// Returns:   bool
//
// Called by: main
// Calls:     none
//
bool redirect_wait(char*& inn, char*& outt, int num_args, char** args, int & first_flag)
{ 

  //check for & flag
  bool amp = (*args[num_args-1] == '&');

  //get redirection files
  for(int i=0; i<num_args; i++)
  {
    if(*args[i]== '>')
    {
      outt = args[i+1];

      if(first_flag == 0)
        first_flag = i;
    }
    else if(*args[i]== '<')
    {
      inn = args[i+1];

      if(first_flag == 0)
        first_flag = i;
    }

  }

  if(first_flag == 0)
    first_flag = num_args;

  return amp;

}

//////////////////////////////////////////////////////////////// 
// parse separtes string input line unto command and args found on line
// in place of tokenizer
// Parameters:  (char* buf, char** args, int & num_args)
//   
//
// Pre:       passed in a comand list w/ args
//             
//             
//
// Post:      list separated into command and args
//             
//             
//
// Returns:   void
//
// Called by: main
// Calls:     none
//
// http://www.cs.cf.ac.uk/Dave/C/node22.htm
void parse(char* buf, char** args, int & num_args)
{
  while (*buf != NULL)
  {

    while ((*buf == ' ') || (*buf == '\n'))
      *buf++ = NULL;

    /*
     * Save the argument.
     */
    *args++ = buf;

    /*
     * Skip over the argument.
     */
    while ((*buf != NULL) && (*buf != ' ') && (*buf != '\n'))
      buf++;

    num_args++;
  }
  num_args--;
  *args = NULL;
}

//////////////////////////////////////////////////////////////// 
// execute create child process forc and exevp comand
//
// Parameters:
//   (char** args, char* path, char* inn, char* outt)
//
// Pre:       command input is parsed
//            
//            
//
// Post:      command executes and waits
//             
//             
//
// Returns:   void
//
// Called by: main
// Calls:     execv system call fork system call 
//
void execute(char** args, char* path, char* inn, char* outt)
{
  int pid, status, fid;

  const char* PATH=path;
  const char* IN =inn;
  const char* OUT=outt;

  //create child
  if ((pid == fork()) < 0)
  {
    //exception
    perror("fork");
    exit(1);
  }

  //child process
  if (pid ==0)
  {
    //check for redirection
    if(inn != NULL)
    {
      close(0);
      fid = open(IN, O_RDONLY | O_NONBLOCK, 00222);
    }

    if(outt != NULL)
    {
      close(1);
      fid = open(OUT, O_WRONLY | O_RDONLY | O_CREAT, 00666);
    }

    //execute command
    execv(PATH, args);

    //end redirection
    if(inn != NULL)
      close(0);
    if(outt != NULL)
      close(1);

    perror(*args);
    exit(0);
  }
  else
    waitpid(pid, NULL, 0);

}

//////////////////////////////////////////////////////////////// 
// execute_a create child process forc and exevp comand
//
// Parameters:
//   (char** args, char* path, char* inn, char* outt)
//
// Pre:       command input is parsed
//            
//            
//
// Post:      command executes no wait
//             
//             
//
// Returns:   void
//
// Called by: main
// Calls:     execv system call fork system call 
//
void execute_a(char** args, char* path, char* inn, char* outt)
{
  int pid, status, fid;

  const char* PATH =path;
  const char* IN =inn;
  const char* OUT=outt;


  //create child
  if ((pid == fork()) < 0)
  {
    //exception 
    perror("fork");
    exit(1);
  }

  //child process
  if (pid ==0)
  {
    //check for redirection
    if(inn != NULL)
    {
      close(0);
      fid = open(IN, O_RDONLY | O_NONBLOCK, 00222);
    }

    if(outt != NULL)
    {
      close(1);
      fid = open(OUT, O_WRONLY | O_RDONLY | O_CREAT, 00666);
    }

    //execute commands
    execv(PATH, args);

    //end redirection
    if(inn != NULL)
      close(0);
    if(outt != NULL)
      close(1);

    perror(*args);
    exit(0);
  }
  else
    return;

}

//////////////////////////////////////////////////////////////// 
// f_path find path of given file name or directories
//
// Parameters:
//   (char** args)
//
// Pre:       command input is parsed
//            
//            
//
// Post:      path to file returned
//             
//             
//
// Returns:   char *
//
// Called by: main
// Calls:     strtok strcpy
//
char* f_path(char** args)
{  
  int i =0;          //index    
  char* s_path= (char*) malloc (2048);    //path vairable
  char* parsed_path [1000];      //array of paths
  char path[1024];        //unparsed line from get PATH

  strcpy(path, getenv("PATH"));    

  char* p = strtok(path, ":");
  parsed_path[i] = p;

  //loop until mo more paths to search or path is found
  while(p != NULL)
  {  //command is in path form of only the filename
    if(*args[0] != '.' && *args[0] != '/')
    {
      strcpy(s_path, parsed_path[i]);
      if(s_path[0] == '.')
        strcpy(s_path, get_current_dir_name());

      strcat(s_path, "/");
      strcat(s_path, args[0]);
      if(!access(s_path, F_OK))
      {
        if(!access(s_path, X_OK))
        {
          cout << s_path << " is executable" << endl;
          return s_path;
        }
        else
          cout << s_path << " is not executable" << endl;
      }
      else
        cout << s_path << " File not found " << endl;

    }
    else if(*args[0] =='/')  //command in form of /filename
    {
      if(!access(args[0], F_OK))
      {
        if(!access(args[0], X_OK))
        {
          cout  << args[0] << " is executable"<< endl;
          return(args[0]);
        }
        else
        {
          cout << args[0] << " is not executable" << endl;
          return(NULL);
        }
      }
      else
      {
        cout << args[0] << " File not found " << endl;
        return NULL;
      }

    }
    else if(args[0][1] == '/')//command is in form of./filename
    {
      args[0]= &args[0][2];
      strcpy(s_path, getenv("PWD"));
      strcat(s_path, "/");
      strcat(s_path, args[0]);
      if(!access(s_path, F_OK))
      {
        if(!access(s_path, X_OK))
        {
          cout << s_path  << " is executable" << endl;
          return(s_path);
        }
        else
        {
          cout << s_path << " is not executable" << endl;
          return(NULL);
        }
      }
      else
      {
        cout << s_path << " File not found" << endl;
        return NULL;
      }

    }
    else  //command is given in the form ../filename
    {
      args[0] = &args[0][3];
      strcpy(s_path, getenv("PWD"));

      int dec = strlen(s_path);    //decrement counter

      for(int j= dec; j>0; j--)
      { 
        if(s_path[j] == '/')
          break;
        s_path[j]=NULL;
      }

      strcat(s_path, args[0]);
      if(!access(s_path, F_OK))
      {
        if(!access(s_path, X_OK))
        {
          cout << s_path << " is executable " << endl;
          return(s_path);
        }
        else
        {
          cout << s_path << " is not executable" << endl;
          return NULL;
        }
      }
      else 
      {
        cout << s_path << " File not found " << endl;
        return NULL;
      }
    }
    i++;
    p= strtok(NULL, ":");
    parsed_path[i] = p;
  }

  return NULL;
}


