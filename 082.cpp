#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int P = 3,
          C = 10,
          N = 6,
          J = 4;
int A[N][J];

vector<int> posV;
vector<int> valV;
vector<int> cosV;
vector<int> plrV;

using namespace std;

/**
 *
 */
void readA()
{
  ifstream is("in.txt");

  for(int i = 0; i < N; ++i)
    for(int j = 0; j < J; ++j)
      is >> A[i][j];

  is.close();
}

/**
 *
 */
void readF(string pName = "in.txt")
{
  int val = 0;

  ifstream is(pName.c_str());

  if(is.is_open())
  {
    is >> val;

    while(!is.eof())
    {
      plrV.push_back(val);
      is >> val;
      valV.push_back(val);
      is >> val;
      cosV.push_back(val);
      is >> val;
      posV.push_back(val);
      is >> val;
    }

    is.close();

    for(int j = 0; j < N; ++j)
    {
      cout << "plrV[" << j << "] : " << plrV[j]
           << "  valV[" << j << "] : " << valV[j]
           << "  cosV[" << j << "] : " << cosV[j]
           << "  posV[" << j << "] : " << posV[j] << endl;
    }
  }
  else
    cerr << "Error: Could not open " << pName << " for reading\n";
}

/**
 *
 */
void printA()
{
  cout << "Rendering A[" << N << "][" << J <<"]\n\n";
  for(int i = 0; i < N; ++i)
  {
    for(int j = 0; j < J; ++j)
    {
      cout << A[i][j];
      if(j + 1 < J) cout << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

/**
 *
 */
void printV(int** & V)
{
  cout << "Rendering V[" << P << "][" << N <<"]\n\n";
  for(int i = 0; i < P; ++i)
  {
    for(int j = 0; j < N; ++j)
    {
      cout << V[i][j];
      if(j + 1 < N) cout << "  ";
    }
    cout << endl;
  }
  cout << endl;
}

/**
 *
 */
void buildV(int** & V)
{
  vector<int> members;
  for(int pos = 0; pos < P; ++pos)
  {
    for(int ccos = 0; ccos < N; ++ccos)
    {
      if(pos == 0 && posV[ccos] == pos+1)
      {
        if(ccos == 0)
        {
          V[pos][ccos] = cosV[pos];
          members.push_back(ccos+1);
        }
        else if (valV[ccos] >= valV[ccos -1] && cosV[ccos] <= C)
        {
          V[pos][ccos] = cosV[pos];
          members.push_back(ccos+1);
        }
        else
        {
          V[pos][ccos] = V[pos][ccos -1];
          members.push_back(ccos+1);
        }
      }
      else
      {
        int prcos, tcos, prval;

        if(ccos > 0)
        {
          prcos = V[pos][ccos-1];
          prval = valV[ccos];
        }
        else
        {
          prcos = V[pos-1][N-1];
          prval = -1;
        }

        tcos = prcos + cosV[ccos];

        if(posV[ccos] == pos+1)
        {
          if(tcos <= C && valV[ccos] >= prval)
          {
            V[pos][ccos] += cosV[ccos];
            for(int ncos = ccos; ncos < N; ++ncos)
            {
              if(tcos > V[pos][ncos])
                V[pos][ncos] = tcos;
            }
            members.push_back(ccos+1);
          }
          else
            V[pos][ccos] = prcos;
        }
        else
          V[pos][ccos] = prcos;
        printV(V);
      }
    }
  }

  for(int i = 0; i < members.size(); ++i)
    V[i][0] = members[i];
}

/**
 *
 */
void retrieveS(int** & V, vector<int> & r)
{
  for(int i = 0; i < P; ++i)
    r.push_back(V[i][0]);
}

/**
 *
 */
int main(int argc, char** argv)
{

  int** V;

  V = new int*[P];

  for(int i = 0 ; i < P; ++i)
    V[i] = new int[N];

  for(int i = 0; i < P; ++i)
    for(int j = 0; j < N; ++j)
      V[i][j] = 0;

  printV(V);
  readF();
  populateV();
  buildV(V);
  printV(V);

  vector<int> r;
  retrieveS(V, r);
  for(int k = 0; k < P; ++k)
    cout << " Player: " << r[k] << " is selected.\n";

  return 0;
}
