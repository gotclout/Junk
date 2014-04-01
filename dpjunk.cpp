#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


string x;
string w;


int rows,
    cols;

void printN(int** & N, int m, int p)
{
  cout << "Rendering N[" << m << "][" << p <<"]\n\n";
  for(int i = 0; i < m; ++i)
  {
    for(int j = 0; j < p; ++j)
    {
      cout << N[i][j];
      if(j + 1 < p) cout << "  ";
    }
    cout << "\n";
  }
}

void buildN(int** & N, int m, int p)
{
  int xp = 0,
      wp = 0,
      pr = 0;

  size_t pos = 0;

  for(int i = 0; i < m; ++i)
  {
    for(int j = 0; j < p; j++)
    {
      if(i == 0)
        N[i][j] = 0;
      else
      {
        pr = N[i-1][j];
        if(x[i] == w[j])
          N[i][j] += pr;
        else
          N[i][j] = 0;
      }
    }
  }
}

int main(int argc, char** argv)
{

  if(argc == 3)
  {
  }
  else
  {
    x = "abcabcababac";
    w = "abc";
  }

  int m = x.length(),
      p = w.length(),
      i = 0;

  int** N;
  N = new int*[m];

  for( ; i < m; ++i)
    N[i] = new int[p];

  for(i = 0; i < m; ++i)
  {
    for(int j = 0; j < p; ++j)
    {
      N[i][j] = 0;
    }
  }

  printN(N, m, p);
  buildN(N, m, p);
  return 0;
}

