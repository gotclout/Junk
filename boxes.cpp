#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

#define S 100
#define K 2  //A box has K dimensions
#define N 10 //There are N boxes of K dimensions
int D[N][K]; //D[1..n, 1..k] is an array of n k dimensional boxes
             //st D[i, j] gives length of box i in dimension j
             //for 1 <= i <= n and 1 <= j <= k, D[i,j] <= D[i, j+1]
             //i.e. each dimension k is equal to or smaller than the next dimension
             //and for 1 <= i < n, D[i, 1] >= D[i+1, 1]
             //i.e. the first dimension of a box is larger or equal to
             //the first dimension of the next box
int cost[N];
using namespace std;

void makeD()
{
  for(int i = 1; i <= N; i++)
  {
    for(int j = 1; j <= K; j++)
    {
      D[i][j] = (rand() % S) + 100;
      if (i > 1 && j == 1)
      {
        while(D[i][j] > D[i-1][j]) D[i][j] = D[i-1][j] - 10;
        if(D[i][j] < 1)
          D[i][j] = 1;
      }
      if(j > 1)
        while(D[i][j] < D[i][j-1]) D[i][j] = D[i][j-1] + 10;
    }
  }
}

bool fits(int next, int prev)
{
  for(int i = 1; i <= K; i++)
    if(D[next][i] >= D[prev][i])
      return false;
  return true;
}

void printD(int prev, int next)
{
  for(int j = 1; j <= K; j++)
    cout << "D[" << prev << "][" << j << "] = " << D[prev][j] << endl;
}

void printC()
{
  for(int i = 1; i <= N; i++)
    cout << "cost[" << i << "] = " << cost[i] << endl;
}

void printD()
{
  for(int i = 1; i <= N; i++)
  {
    cout << "[" << i << ", ";
    for(int j = 1; j <= K; j++)
    {
      cout << D[i][j];
      if(j != K) cout << ", ";
    }
    cout << "]\n";
  }
}
#define max(left, right) left > right ? left : right

int costD()
{
  int M = 1;
  for(int k = 1; k <= N; k++)
    cost[k] = 0;
  for(int i = 1; i <= N; i++)
  {
    cost[i] = 1;
    for(int j = 1; j <= i; j++)
    {
      if(fits(i, j))
      {
        cost[i] = max(cost[i], cost[j]+1);
        if(cost[i] > M) M = cost[i];
        cout << "box " << i << " fits into box " << j << endl;
      }
      else
        j = N+1;
    }
  }
  printC();
  return M;
}

int main(int argc, char** argv)
{
  srand(time(0));
  makeD();
  printD();
  cout << "M: " << costD() << endl;
  return 0;
}
