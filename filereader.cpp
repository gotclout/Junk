#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 10
#define MAX_LEN 256

bool appendStringToFile(const char* stringToAppend, const char* filePath, const char delimiter)
{
  bool retVal = true;
  FILE* file = 0;
  file = fopen(filePath, "a+" );
  if(file){
    switch(delimiter){
      case 's':
        fprintf( file, "%s%s", stringToAppend, " " );
        break;
      case 't':
        fprintf( file, "%s%s", stringToAppend, "\t" );
        break;
      case 'n':
        fprintf( file, "%s%s", stringToAppend, "\n" );
        break;
      default:
        printf("You have entered and invalid delimiter: %c", delimiter);
        retVal = false;
        break;
    }
  }

  fclose(file);
  return retVal;
}

char* getStringAtIndexInFile( int index, char const *filePath) {
  FILE *file = 0;
  char* buf = 0;
  file = fopen(filePath, "r" );
  if(index < 1){
    printf("Index must be greater than 0\n");
  }
  else if(file){
    buf = new char[MAX_LEN];
    memset(buf, 0, MAX_LEN);
    for ( int i = 0; i < index; i++ ) {
      fscanf(file, "%s ", buf);
    }
  }
  else{
    buf = "File not found\n";
  }

  fclose( file );
  return buf;
}

bool unitTest()
{
  bool	retVal = false;
  int		i;
  char	testString[MAX_LEN];
  char	*spaceFile = "SpaceUnitTestFile.txt", 
        *tabFile = "TabUnitTestFile.txt",
        *newLineFile = "NewLineUnitTestFile.txt",
        *result;

  //test appending
  memset(testString, 0, MAX_LEN);
  for(i = 0; i < MAX_STRINGS; i++){
    strcat(testString, "10");
    retVal = appendStringToFile(testString, spaceFile, 's');
    retVal = appendStringToFile(testString, tabFile, 't');
    retVal = appendStringToFile(testString, newLineFile, 'n');
  }

  //test retrieveing
  memset(testString, 0, MAX_LEN);
  for(i = 1; i <= MAX_STRINGS; i++){
    strcat(testString, "10");
    result = getStringAtIndexInFile(i, spaceFile);
    if(strcmp(result, testString))
      retVal = false;
    result = getStringAtIndexInFile(i, tabFile);
    if(strcmp(result, testString))
      retVal = false;
    result = getStringAtIndexInFile(i, newLineFile);
    if(strcmp(result, testString))
      retVal = false;
  }

  //invalid delimiter
  getStringAtIndexInFile(1, tabFile);

  //file not found
  getStringAtIndexInFile(1, "INVALIDFILE");

  //negative index
  getStringAtIndexInFile(-1, tabFile);

  return retVal;
}

int main( int argc, char *argv[] ) {
  unitTest();
  if(argc > 1){
    if(!strcmp(argv[1], "a") && argc == 5){
      appendStringToFile(argv[3], argv[4], argv[2][0]);
    }
    else if (!strcmp(argv[1], "g") && argc == 4){
      char* str = getStringAtIndexInFile(atoi(argv[2]), argv[3]);
      if(str){
        printf("%s", str);
        delete str;
      }
    }
    else if(strcmp( argv[1], "g") &&  strcmp( argv[1], "a")){
      printf("You have entered an invald opperation\n");
    }
    else{
      printf("You hve not provided the necessary arguments\n");
    }
  }
  else{
    printf("Program arguments:\nUse 'a' to append to a file, provide filename and delimiter (s, t, n)\n"
        "Use 'g' to retrieve from a file and provide index and filename\n");
  }

  return 0;
}




