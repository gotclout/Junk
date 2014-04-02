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

int buildN(int** & N, int m, int p)
{
  int xp = 0,
      wp = 0,
      pr = 0,
      hz = 0;

  size_t pos = 0;

  for(int i = 0; i < m; ++i)
  {
    for(int j = 0; j < p; j++)
    {
      //cout << "x[i] ~ " << x[i] << " : w[j] ~ " << w[j] << endl;
      if(i == 0 && x[i] == w[j])
        N[i][j] = 1;
      else if(i == 0) N[i][j] = 0;
      else
      {
        if(j > 0)pr = N[i-1][j-1];
        else pr = 0;
        cout << "prev: " << pr << endl;
        if(x[i] == w[j])
        {
          cout << "match: x[" << i << "] : " << x[i]
               << "\tw[" << j << "] : " << w[j] << endl;
           N[i][j] += pr + 1;
          if(N[i][j] == p)
          {
            cout << "\tN[i][j] ~ " << N [i][j]
                 << " | N[i][j] % " << p << " ~  " << N[i][j] % p << endl;
            hz++;
            cout << "hz: " << hz << endl;
          }

        }
        else
          N[i][j] = 0;
      }
    }
  }

  return hz;
}

int main(int argc, char** argv)
{

  if(argc == 3)
  {
  }
  else
  {
    x = "abcabcaabcccabc";
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
  int freq = buildN(N, m, p);
  printN(N, m, p);
  cout << "Freq: " << freq << endl;

  return 0;
}

