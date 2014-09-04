#include <iostream>
#include <string>

using namespace std;

const int n = 7;

int partition_r(int A[], const int x, const int sz = 0)
{
  int m = (sz / 2) + 1;
  cout << "x : " << x << " sz : " << sz << " m : " << m << endl;
  if(m == 1 && sz == 0 && A[m] > x) return 0;
  else if(m < n && (A[m-1] <= x && A[m+1] > x))
    return m;
  //else if ( sz == 0 && x == 1) return 0;
  else if(m > n - 1)
    return A[n-1] + 1;
  else if(A[m] <= x)
    m = ((m + 1) / 2) + 1 ;
  else
    m = ((m - 1) / 2) + 1;
  return partition_r(A, A[m], m);
}

int partition(int A[], const int x)
{
  int m = -1,
      i = 1;

  cout << "A: ";
  for( ; i < n; ++i)
    (i + 1) == n ? cout << A[i] << endl : cout << A[i] << " ";

  m = (n / 2) + 1;

  while(!(A[m] <= x && A[m+1] > x) && m > 0 && m < n )
  {
    if(m == 1)
      m = 0;
    else if(m > n - 1)
      m = n - 1;
    else if(A[m] <= x)
          m = ((m + 1) / 2) + 1 ;
    else// if(m < 2)
      m = ((m - 1) / 2) + 1;
    cout << "m: " << m << endl;
  }
  return m;
}

int main(int argv, char** argc)
{
  int A[7] = {-1, 2, 4, 5, 5, 8, 9};
  cout << "begin iterative" << endl;
  int q    =
    partition(A, 5);
  cout << "Partition Returned Q: " << q << endl; //4
  q = partition(A, 6);
  cout << "Partition Returned Q: " << q << endl;
  q = partition(A, 7);
  cout << "Partition Returned Q: " << q << endl;
  q = partition(A, 4);
  cout << "Partition Returned Q: " << q << endl; //2
  q = partition(A, 3);
  cout << "Partition Returned Q: " << q << endl; //1
  q = partition(A, 2);
  cout << "Partition Returned Q: " << q << endl;
  q = partition(A, 1);
  cout << "Partition Returned Q: " << q << endl; //0

  cout << "begin recurssive" << endl;
  q = partition_r(A, 5, 7);
  cout << "Partition R Returned Q: " << q << endl;
  q = partition_r(A, 6, 7);
  cout << "Partition R Returned Q: " << q << endl;
  q = partition_r(A, 7, 7);
  cout << "Partition R Returned Q: " << q << endl;
  q = partition_r(A, 4, n);
  cout << "Partition R Returned Q: " << q << endl;
  q = partition_r(A, 3);
  cout << "Partition R Returned Q: " << q << endl;
  q = partition_r(A, 2);
  cout << "Partition R Returned Q: " << q << endl;
  q = partition_r(A, 1);
  cout << "Partition R Returned Q: " << q << endl;

  return 1;
}

                                           
