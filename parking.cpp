#include <iostream>
#include "timer.h"

#define N 7

using namespace std;

void print(const int* a)
{
  for(int i = 0; i < N; i++)
    cout << a[i] << " ";
  cout << endl;
}

int match(const int* a, const int* b)
{
  int i = 0;
  while(i < N && a[i] == b[i])i++;
  return (i == N) ? -1 : i;
}

void swap(int* a, const int* b, const int & idx)
{
  int zpos = 0, vpos = 0;
  while(zpos < N && a[zpos] != 0)zpos++;
  while(vpos < N && a[vpos] != b[idx])vpos++;

  if(zpos == idx)
  {
    a[idx] = b[idx];
    a[vpos] = 0;
  }
  else
  {
    a[zpos] = a[idx];
    a[idx] = b[idx];
    a[vpos] = a[zpos];
    a[zpos] = 0;
  }
}

/**
 *  TODO: Doc
 */
int main(int argc, char** argv)
{
  timer t;
  t.Start();
  int a[] = {0, 1, 2, 3, 4, 6, 5};
  int b[] = {4, 3, 2, 5, 6, 1, 0};

  cout << "initial a: ";
  print((const int*)a);
  cout << "final   b: ";
  print((const int*)b);

  for(int m = match(a, (const int*)b); m != -1; m = match(a, (const int*)b))
    swap(a, (const int*)b, m);

  cout << "final   a: ";
  print((const int*)a);
  t.Stop();
  cout << t.Duration() << "\nms\n";
  return 0;
}
