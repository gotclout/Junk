#include <iostream>
#include <fstream>
int fmin(int a, int b);

using namespace std;
int main()
{
  const int n=80;
  int cost[n][n], temp=0, k=1, l=0;
  char trash='*';
  ifstream in; in.open("matrix.txt");
  //init to input file
  for(int i=0; i<n; i++)  
  {
    for(int j=0; j<n; j++)
    {
      in >> temp;
      if(j!=n-1)
        in >> trash; 
      cost[i][j] = temp;
    }
  }
  //build cost matrix
  while(!(k==n-1 && l == n-1))
  {
    if(l == 0)
      ;
    else if(k >0 && l > 0 && k<n-1)
    {	
      int v = INT_MAX, u=0;
      for(int t=k+1; t<n; t++)
      {
        if(fmin(v, cost[t][l]+cost[t][l-1]) < v)
        {
          for(int z=k+1; z<t; z++)
            u+=cost[z][l];
          if(u+cost[t][l]+cost[t][l-1]<v)
            v = fmin(v, cost[t][l]+cost[t][l-1])+u;
          u=0;
        }
      }
      cost[k][l] += fmin(fmin(cost[k][l-1], cost[k-1][l]), v);
    }
    else if(k==0)
    {
      int v = INT_MAX, u=0;
      for(int t=k+1; t<n; t++)
      {
        if(fmin(v, cost[t][l]+cost[t][l-1]) < v)
        {
          for(int z=k+1; z<t; z++)
            u+=cost[z][l];
          if(u+cost[t][l]+cost[t][l-1]<v)
            v = fmin(v, cost[t][l]+cost[t][l-1])+u;
          u=0;
        }
      }
      cost[k][l] += fmin(cost[k][l-1],v);
    }
    cost[k][l] += fmin(cost[k][l-1],cost[k-1][l]);
    //next index
    if(k == n-1)
    {
      k=0;
      l++;
    }
    else
      k++;

  }
  cost[k][l] += fmin(cost[k][l-1],cost[k-1][l]);
  int min=INT_MAX;

  for(int z=0; z<n; z++) 
    min = fmin(min, cost[z][n-1]);
  cout << min << endl;

  return min;
}
int fmin(int a, int b)
{

  if(a > b )
    return b;
  else
    return a;
}
