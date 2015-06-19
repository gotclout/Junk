#include <iostream>
#include "stdlib.h"
#include "string.h"

using namespace std;

/**
 * Reverse space delimited substrings in str in place
 */
char* reverse_words(char* str)
{
  if(str == NULL) return str;

  size_t subsz = 0, strsz = strlen(str);

  for( char* sub = strtok(str, " "); sub != NULL; sub = strtok(NULL, " ") )
  {
    subsz = strlen(sub);

    for(size_t i = 0; i < subsz / 2; ++i )
    {
      str[strsz] = sub[i];
      sub[i] = sub[subsz - 1 - i];
      sub[subsz - 1 - i] = str[strsz];
    }

    sub[subsz] = ' ';
    str[strsz] = 0;
  }

  return str;
}

/**
 * Exec reverse words
 */
int main(int argc, char** argv)
{
  char s [] = "abc def dog.";
  char* r = reverse_words((char*) s);

  if(r) cout << r << '\n';

  return 0;
}
