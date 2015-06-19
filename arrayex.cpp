#include <iostream>
#include "string.h"
#include "stdio.h"

#define len 5

using namespace std;

/**
 * Reverse substring in place
 */
char* reverse_words(char* & str)
{
  // IN:  "abc def dog."  -->
  // OUT: "cba fed .god"

  if(str == NULL) return str;

  size_t sz = 0, strsz = strlen(str), i;

  for( char* sub = strtok(str, " "); sub != NULL; sub = strtok(NULL, " ") )
  {
    sz = strlen(sub);

    for( i = 0; i < sz / 2; ++i )
    {
      str[strsz]      = sub[i];
      sub[i]          = sub[sz - 1 - i];
      sub[sz - 1 - i] = str[strsz];
    }

    sub[sz]    = ' ';
    str[strsz] = 0;
  }

  return str;
}

/**
 * Reverse a cstring in place
 */
void reverseinplace(const char* c, int i = 0)
{
  cout << "REVF\n";
  int sz = strlen(c);

  cout << "  STR: " << c << "\n  LEN: " << sz << '\n';
  char* cp = new char[sz + 1]; // dynamic string array
  memset(cp, 0, sz + 1);
  memcpy(cp, c, sz);

  for( ; i < sz / 2 ; ++i)
  {
    cp[sz] = c[i];
    cp[i] = c[sz - 1 - i];
    cp[sz - 1 - i] = c[sz];
  }

  cp[sz] = 0;
  cout << "  REV: " << cp << '\n';
  delete [] cp;
}

/**
 * Initialize an array
 */
void arrayinit(int a[], int sz, int i = 0)
{
  cout << "INITF\n";
  for(; i < sz; ++i) a[i] = rand();
  cout << "    a: ";
  for(i = 0; i < sz; ++i) cout << a[i] << ' ';
  cout << '\n';
}

/**
 * Perform dynamic array operations
 */
void arrayptrs(int a[], int* & a1, int** & a2, int sz)
{
  cout << "PTRF\n";
  int i = 0, j = 0;
  a1 = new int[len]; //dynamic int array
  for(i = 0; i < sz; ++i)  a1[i] = a[i];
  cout << "   a1: ";
  for(i = 0; i < sz; ++i) cout << a1[i] << ' ';
  cout << '\n';

  int a3[sz][sz]; //2d array
  for(i = 0; i < sz; ++i)
    for( j = 0; j < sz; ++j)
      a3[i][j] = a1[j];

  for(i = 0; i < sz; ++i)
  {
    cout <<  "a3[" << i << "]: ";
    for(j = 0; j < sz; ++j)
      j + 1 == sz ? cout << a3[i][j] << '\n' : cout << a3[i][j] << ' ';
  }

  delete [] a1;
}

/**
 * Perform some cstring ops
 */
void cstrops()
{
  cout << "OPSF\n";
  const char* s = "The quick brown fox jumps over the...";

  char a[strlen(s) + 1];  //copy
  memset(a, 0, strlen(s) + 1);
  strcpy(a, s);

  char b[strlen(s) + 1];  //safe copy
  memset(b, 0, strlen(s) + 1);
  strncpy(b, a, strlen(a));

  char* sub = strstr(s, "brown"); //substr at str
  cout << "  STR: " << s << "\n  SUB: brown\n  RES: " << sub << '\n';

  char* c = strchr(s, 'q'); //substr at char
  cout << "  STR: " << s << "\n  CHR: q\n  RES: " << c << '\n';

  char* rs = strrchr(s, 'u'); //reverse substr at char
  cout << "  STR: " << s << "\n  CHR: u\n  RES: " << rs << '\n';
}

/**
 * Perform some char array io operations
 */
void cstrout(const char* s)
{
  cout << "PRINTF\n";
  size_t sz = strlen(s);
  const char* fmt = "%s%s";
  cout << "  STR: " << s << "\n  FMT: " << fmt << "\n  LEN: " << sz << '\n';
  char a5[sz + 1];
  memset(a5, 0, sz + 1);
  sprintf(a5, fmt, s);
  printf(fmt, "   a5: ", a5);
  sz += strlen("   a6: ") + 1;
  char a6[sz + 1];
  memset(a6, 0, sz + 1);
  snprintf(a6, sz + 1, fmt, "   a6: ", s);
  printf("\n%s\n", a6);
}

/**
 * An example of performing array operations
 */
int main(int argc, char* argv[])
{
  srand(time(0));

  int a[len];  //int array
  arrayinit(a, len);
  int* a1; //dynamic int array
  int** a2 = &a1; //pointer to int array
  arrayptrs(a, a1, a2, len);
  const char* c = "Hello world!";
  reverseinplace(c);
  cstrout(c);
  cstrops();

  return 0;
}

