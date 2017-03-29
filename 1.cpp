#include <iostream>
#include <string>

using namespace std;

int r_partition(int A[], int x, int lo, int hi)
{
  int mid = (hi + lo) / 2;

  if(lo > hi)
    return hi;
  else if(A[mid-1] <= x && A[mid ] > x)
    return mid - 1;
  else if(x < A[mid])
    return r_partition(A, x, lo, mid - 1);
  else if(x >= A[mid])
    return r_partition(A, x, mid + 1, hi);
}

int i_partition(int A[], int x, int lo, int hi)
{
  if(x < A[lo])
    return 0;
  else if(x >= A[hi])
    return hi;

  int q = -1,
      mid;

  while(q < 0)
  {
    // mid =  (hi + lo) / 2; ?overflow
    mid =  lo + (hi + lo) / 2;

    if(A[mid-1] <= x && A[mid] > x)
      q = mid - 1;
    else if(x < A[mid])
      hi = mid - 1;
    else if(x >= A[mid])
      lo = mid + 1;
  }

  return q;
}

void test(int A[], const int n)
{
  cout << "Begin...\n";
  int    i = 1, qi, qr;

  cout << "A: ";
  for( ; i < n; ++i)
    (i + 1) == n ? cout << A[i] << endl : cout << A[i] << " ";

  cout << "\nTesting Partition...\n";
  for(i = 1; i < 11; ++i)
  {
    cout << "RPartioning...\n";
    qr = r_partition(A, i, 1, n-1);
    cout << "IPartioning...\n";
    qi = i_partition(A, i, 1, n -1);
    cout << "\nRPartition " << i << " Returned Q: " << qr << endl;
    cout << "\nIPartition " << i << " Returned Q: " << qi << endl;
  }

  cout << "End...\n";
}

int main(int argc, char** argv)
{
  int A0[7] = {-1, 2, 4, 5, 5, 8, 9};
  int A1[8] = {-1, 1, 1, 5, 5, 7, 7, 9};

  // 1 : 0 | 2 3 :1 | 4 : 2 | 5 6 7 : 2
  test(A0, 7);
  test(A1, 8);

  return 1;
}
