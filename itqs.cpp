#include <iostream>
#include <string>
#include "stdio.h"

using namespace std;

void printstairs(int n)
{
  int i, j, k;
  for (i = 1; i <= n; i++)
  {
    for( j = 1; j <= n - i; j++)
    {
      cout << " ";
    }
    for( k = j ; k <= n ; k++)
    {
      cout << "#";
    }
    cout << endl;
  }
}

void inplaceswap(int & a, int & b)
{
  cout << "a:" << a << " b:" << b << endl;
  a = a ^ b;
  cout << "a:" << a << " b:" << b << endl;
  b = a ^ b;
  cout << "a:" << a << " b:" << b << endl;
  a = a ^ b;
  cout << "a:" << a << " b:" << b << endl;
}

void io()
{
  scanf("%i", &n);
  printf("%i", n);
}

int main()
{
  printstairs(6);

  cout << endl;

  int a = 7, b = 3;
  inplaceswap(a, b);
  cout << "a:" << a << " b:" << b << endl;
  io();

  return 0;
}
