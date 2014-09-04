#include <iostream>
#include <string>

using namespace std;


int apartition(int A[], int x, int lo, int hi)
{
  int mid =  lo + ((hi - lo) / 2);

  cout << "X:   " << x   << "\n"
       << "MID: " << mid << " " << A[mid]<< "\n"
       << "LO:  " << lo  << " " << A[lo] << "\n"
       << "HI:  " << hi  << " " << A[hi] << "\n";

  if(mid == hi && A[mid] > x)
    return mid - 1;
  else if(mid == lo && A[mid] > x)
    return 0;
  else if(mid == lo)
    return mid;
  else if(A[mid-1] <= x && A[mid ] > x)
    return mid - 1;
  else if(x < A[mid])
    return apartition(A, x, lo, mid - 1);
  else if(x >= A[mid])
    return apartition(A, x, mid + 1, hi);
}

int r_partition(int A[], const int x, const int sz = 0, const int n = 0)
{
  int m = (sz / 2) + 1;
  cout << "x : " << x << " sz : " << sz << " m : " << m
       <<  " A[m] : " << A[m] << endl;
  if(m == 1 && A[m] > x) return 0;
  else if(x > A[n - 1]) return n;
  else if(A[m-1] <= x && A[m+1] > x) return m;
  else if(A[m] <= x) m = ((m+1)/2) + 1;// m = m + ((m + 1) / 2) + 1 ;
  else m = (sz - m)/2;

  return r_partition(A, x, m, n);
}

int i_partition(int A[], const int x, const int n)
{
  int m = -1,
      i = 1;

  m = (n / 2) + 1;

  cout << "x : " << x << " m : " << m <<  " A[m] : " << A[m] << endl;
  while(!(A[m] <= x && A[m+1] > x) && m > 0 && m < n )
  {
    if(m == 1 && A[m] > x) m = 0;
    else if(x > A[n - 1]) m = n;
    else if(A[m] <= x) m = ((m + 1) / 2) + 1 ;
    else m = ((m - 1) / 2) + 1;
    cout << "x : " << x << " m : " << m <<  " A[m] : " << A[m] << endl;
  }
  return m;
}

void test(int A[], const int n)
{
  cout << "Begin...\n";
  int    i = 1, q0, q1;

  cout << "A: ";
  for( ; i < n; ++i)
    (i + 1) == n ? cout << A[i] << endl : cout << A[i] << " ";

  cout << "\nTesting Partition...\n";
  for(i = 1; i < 11; ++i)
  {
    int q = apartition(A, i, 1, n-1);
    /*cout << "IPartioning...\n";
    q0 = i_partition(A, i+1, n);
    cout << "RPartioning...\n";
    q1 = r_partition(A, i+1, n, n);
    cout << "\nIPartition " << i+1 << " Returned Q: " << q0
         << "\nRPartition " << i+1 << " Returned Q: " << q1 << endl;
    */
    cout << "\nIPartition " << i << " Returned Q: " << q << endl;
  }

  cout << "End...\n";
}

int main(int argv, char** argc)
{
  int A0[7] = {-1, 2, 4, 5, 5, 8, 9};
  int A1[8] = {-1, 1, 1, 5, 5, 7, 7, 9};

  // 1 : 0 | 2 3 :1 | 4 : 2 | 5 6 7 : 2
  test(A0, 7);
//  test(A1, 8);

  return 1;
}
