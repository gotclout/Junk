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

using namespace std;

void readA()
{
  ifstream is("in.txt");

  for(int i = 0; i < N; ++i)
    for(int j = 0; j < J; ++j)
      is >> A[i][j];

  is.close();
}

void populateV()
{
  for(int i = 0; i < N; ++i)
  {
    valV.push_back(A[i][1]);
    cosV.push_back(A[i][2]);
    posV.push_back(A[i][3]);
  }

  for(int j = 0; j < N; ++j)
  {
    cout << "valV[" << j << "] : " << valV[j] << endl
         << "cosV[" << j << "] : " << cosV[j] << endl
         << "posV[" << j << "] : " << posV[j] << endl;
  }
}

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

int updateV(int** & V, int plc, int pos, int prc)
{
  int bestc = INT_MAX, bestv = 0, curr = 0,
      i = plc, j = pos, maxv = prc, minc;

  for(  ; i < N; ++i)
  {
    //maxv = max(V[pos][i], V[pos][i+1]);
    //minc = min(A[i][j], A[i][j+1]);
    if(maxv > V[pos][i])
    {
      bestv = maxv;
      V[pos][i] = bestv;
      //for(int j = pos; j < N; ++j)
      //{
        //minc < bestc)
        //curr += V[][];
      //}
    }

  }

  return bestv;
}

int buildV(int** & V)
{
  int best = 0;

  for(int pos = 0; pos < P; ++pos)
  {
    for(int ccos = 0; ccos < N; ++ccos)
    {
      if(pos == 0)
        V[pos][ccos] = cosV[pos];
      else
      {
        int prcos, tcos;

        if(ccos > 0)
          prcos = V[pos][ccos-1];//cosV[ccos];
        else
          prcos = V[pos-1][N-1];

        tcos = prcos + cosV[ccos];

        cout << "position: " << pos << " cost: " << cosV[ccos]
          << " pcost: " << prcos << " total: " << tcos << endl;
        if(tcos <= C)
        {
          V[pos][ccos] += cosV[ccos];
          best = updateV(V, ccos, pos, tcos);
        }
        else
          V[pos][ccos] = prcos;
      }
    }
      printV(V);
  }

  return best;
}

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
  readA();
  printA();
  populateV();
  buildV(V);
  printV(V);

  return 0;
}
