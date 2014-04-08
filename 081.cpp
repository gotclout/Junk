#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


string x;
string w;

/**
 *
 */
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

/**
 *
 */
int buildN(int** & N, int m, int p)
{
  bool match;
  for(int i = 0; i < m; ++i)
  {
    for(int j = 0; j < p; ++j)
    {
      match = w[j] == x[i];
      if(match && j + 1 == p)
        N[i][j] = N[i][j-1] + 1;
      else if(match && j == 0 && i > 0)
        N[i][j] = 1 + N[i-1][p-1];
      else
      {
        if(i > 0 && j == 0)
          N[i][j] = N[i-1][p-1];
        else if(i == 0 && j == 0)
          N[i][j] = 0;
        else
          N[i][j] = N[i][j-1];
      }
    }
    printN(N, m, p);
  }
  return N[m-1][p-1];
}

/**
 *
 */
int main(int argc, char** argv)
{
  if(argc == 3)
  {
  }
  else
  {
    x = "abab";
    w = "ab";
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
  int freq = buildN(N, m, p);
  printN(N, m, p);
  cout << "Freq: " << freq << endl;

  for( i = 0; i < m; ++i)
    delete [] N[i];
  
  delete [] N;
  
  return 0;
}

