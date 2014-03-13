#include<iostream>
#include <iomanip>
#include <fstream>

using namespace std;

const static int n = 4;

int A[n][n];

struct list
{
  bool isEmpty;
  int head;
  list* tail;

  list(int v) { head = v; tail = 0; isEmpty = false;};
  list() { isEmpty = true; tail = 0;};
};

struct amat
{
  int n;

  int**A;

  amat(int n)
  {
    A = new int*[n];
    for(int i = 0; i < n; ++i)
      A[i] = new int[n];
  };

  int get(int i, int j){ return A[i][j];};

  int put(int i, int j, int v)
  {
    A[i][j] = v;
  };

  int* allfrom(int i)
  {
    int* r = new int[n];
    for(int idx = 0; idx < n; i++)
      r[idx] = A[i][idx];

    return r;
  };
};

struct alist
{
  int n;
  list** L;

  alist(int n)
  {
    L = new list*[n];
    memset(L, 0, n*sizeof(list*));
  };

  list* get(int i, int j)
  {
    list* n = L[i];

    do
    {
      if(n->head == j)
        return n;
      else
        n = n->tail;
    }
    while(n);

    return 0;
  };

  void put(int i, int j)
  {
    list* l = 0;
    if(!L[i])
    {
      l = new list(j);
      L[i] = l;
    }
    else
    {
      l = new list(j);
      l->tail = L[i];
      L[i] = l;
    }
  };

  list* allfrom(int i)
  {
    return L[i];
  }
};

alist* al;
alist* ol;
amat* am;

void makeL()
{
  al = new alist(n);
  al->put(0, 1);
  al->put(1, 3);
  al->put(2, 1);
  al->put(3, 2);
}

void printL(bool in = true)
{
  list* l;
  for(int i = 0; i < n; ++i)
  {
    cout << "L[" << i << "] ";
    if(in)
      l = al->allfrom(i);
    else
      l = ol->allfrom(i);
    if(l)
    {
      do
      {
        cout << l->head << " -> ";
        l = l->tail;
      }
      while(l);
      cout << "nil\n";
    }
    else
      cout << "nil\n";
  }

  cout<< '\n';
}

void reverseLL(int i, int & c)
{
  if(c != n)
  {
    ++c;
    list* next = al->allfrom(i);

    if(next)
    {
      int tmp = next->head;
      reverseLL(tmp, c);
      ol->put(tmp, i);
      //L[tmp]->put(i);
  ////    if(L[tmp]->tail)
   //   L[tmp]->tail = 0;
      //L[tmp]->tail->tail = 0;
    }
    /*
    while(next->tail)
    {
      L[next->tail->head]->put(i);
      printL();
      next = L[next->tail]->tail->tai
      reverseLL(next->tail->head);
    }*/
  }
  else
  {
    //list* next = L[i];
    //int tmp = next->tail->head;
    //L[tmp]->tail = 0;
    //L[tmp]->put(i);
    //printL(o);
  }
}

void printA()
{
  cout << "Array A[" << n-1 << "][" << n - 1 << "]\n";
  for(int i = 0; i < n; ++i)
    for(int j = 0; j < n; ++j)
      j < n - 1 ? cout << setw(8) <<  A[i][j] :
              cout << setw(8) <<  A[i][j] << '\n';

  cout << '\n';
}

void readA()
{
  ifstream in;
  in.open("m.txt");
  int tmp = 0;

  for(int i = 0; i < n; ++i)
  {
    for(int j = 0; j < n; ++j)
      in >> A[i][j];
  }

  in.close();
}

void transposeA()
{
  int tmp = 0;
  for(int i = 0; i < n - 1; ++i)
  {
    for(int j = i + 1; j < n; ++j)
    {
      tmp = A[i][j];
      A[i][j] = A[j][i];
      A[j][i] = tmp;
    }
  }
}

void transposeAA()
{
  for(int i = 0; i < n - 1; ++i)
  {
    for(int j = i + 1; j < n; ++j)
    {
      int tmp = am.get(i, j);
      am->put(i, j, am->get(j, i));
      am->put(j, i, tmp);
    }
  }
}

void printAA()
{

}

int main(int argc, char** argv)
{
  readA();

  printA();

  transposeA();

  printA();

  makeL();

  printL();

  //reverseL();

  int c = 0;
  ol = new alist(n);
  reverseLL(0, c);

  printL(false);

  am = new amat(n);

  return 0;
}
