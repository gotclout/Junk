#include <map>
#include <list>
#include <vector>
#include <math.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <limits>

/** Open Source Timer Impl                                                   **/
#include "Timer.h"

/** Timers                                                                   **/
Timer qst, ist, oqt, oit;

int sav[] = {152, 191, 1, 88, 77, 198, 122, 38, 169, 55, 24, 65, 86, 8, 68,
             65, 30, 142, 133, 92, 134};


/** timer sums over cycles                                                   **/
double isum, oisum, qsum, oqsum;

/** no std::nonsense                                                         **/
using namespace std;

/******************************************************************************
 * User defined values: use command line input argv
 *
 *  argv[0]
 *  program name
 *
 *  argv[1]
 *  cycles: indicates number of cycles or iterations the test should be run
 * default: cycles = 1 unless user specified
 *
 *  argv[2]
 *       n: indicates the number elems or size of the container to be sorted
 * default: n = 9 unless user specified
 *
 *  argv[3]
 *       M: indicates the value at which insertion sort may be more efficient
 *          for trivially small containers
 *
 * argv[4]
 *      D: indicates whether or not to use the same values for each cycle
 * default: 0, unused unless user specified
 ******************************************************************************/
static int cycles = 3;
static int n      = 21;
//static int M      = n - (n/2);
static int M      = n - sqrt(n + 0.0);
static bool D     = false;

/** ivec is the Container to be sorted                                       **/
typedef vector<int> ivt;
ivt isv, oqv, oiv, qsv, ivec;

/** A string stream for manipulating string values                           **/
stringstream sstr;

/** the table header                                                         **/
const char* header =
  "   Cycle        Duration    ISort    QSort    ISwap     QSwap\n\n";

/*******************************************************************************
 *    MACRO: string stream manipulation operations
 *
 * strtoval: converts string values to integer value or the type of v
 *   clsstr: initializes the string stream or clears previous data if sstr
 *           has been initialized
 ******************************************************************************/
#define strtoval(str, v) sstr << str; sstr >> v;
#define clsstr() sstr.str(""); sstr.clear();

/**     MACRO to generate random values between 0 and elems * 10           **/
#define genrand() rand() % (n * 10)

/**     MACRO template for sorting any container with template typename     **/
#define value_type_t typename Container::value_type

/**
 * @sstats: a performance statistics object for analysis
 */
struct sstats
{
  /** num swaps in insertion sort **/
  int iswaps,
  /** num swaps in quick sort **/
      qswaps,
  /** num calls to insertion sort **/
      icalls,
  /** num calls to quick sort **/
      qcalls;

  /**
   * Def construct
   */
  sstats() { init(); } ;

  /**
   * init:
   */
  void init() { iswaps = qswaps = icalls = qcalls = 0; } ;

  /**
   * Output operator overload
   */
  friend ostream& operator <<(ostream & o, sstats & s)
  {
    o << "Rendering Current Statistics..."  << endl << endl
      << "insertion sort calls: " << s.icalls << endl
      << " swaps to completion: " << s.iswaps << endl
      << "    quick sort calls: " << s.qcalls << endl
      << " swaps to completion: " << s.qswaps << endl;

    return o;
  };
};

/** Performance Statistics                                                   **/
static sstats stats;

/**
 * swap: replaces the content of values in a container
 *
 * @param size_t lval is the position of the left obj to be swapped
 * @param size_t rval is the position of the right obj to be swapped
 * @return Container c is the modified container
 */
template <class Container>
Container& swap(Container & c, const size_t lval, const size_t rval)
{
  if(lval < c.size() && rval < c.size())
  {
    value_type_t tmp = c.at(lval);
    c.at(lval)       = c.at(rval);
    c.at(rval)       = tmp;
  }

  return c;
}

/**
 * isort: executes insertion sort algorithm
 *
 * @param Container c
 * @param size_t beg
 * @param size_t end
 *
 * @return Container is a copy of the sorted container from beg to end
 */
template <class Container>
Container& isort(Container & c, const size_t beg, const size_t end)
{
  value_type_t x;
  size_t       j,
               sz = end - beg + 1;

  ++stats.icalls;
  for(size_t i = beg; i < sz; ++i)
  {
    x = c.at(i);
    j = i;
    while(j > beg && c.at(j-1) > x)
    {
      c.at(j) = c.at(j-1);
      j = j - 1;
      ++stats.iswaps;
    }
    c.at(j) = x;
  }

  return c;
}

/**
 *
 */
template <class Container>
Container& oisort(Container & c, const size_t beg, const size_t end)
{
  value_type_t x;
  size_t       j,
               sz = end - beg + 1;

  value_type_t tmp;
  ++stats.icalls;
  for(size_t i = beg; i < sz; ++i)
  {
    j = i;
    while(j > beg && c.at(j-1) > c.at(j))
    {
      tmp = c.at(j);
      c.at(j) = c.at(j-1);
      c.at(j-1) = tmp;
      j = j - 1;
      ++stats.iswaps;
    }
  }

  return c;
}

/**
 *
 */
size_t Pivot(const size_t left, const size_t right)
{
  size_t mid = ((right + left/2));

  if(mid == 0) mid = 1;

  if((right - 2) - (left + 2) > 2)
  {
    mid  += ((right - 2) - (left + 2)) ;
    mid /= 2;
  }

  cout <<  "left : " << left << " right : " << right << " piv : " << mid << endl;

  while(mid > n) mid /= 2;
  return mid;
}

/**
 *
 */
template <class Container>
Container& oqsort(Container & c, const size_t left, const size_t right)
{
  size_t i = left,
         j = right;

  if(j - i > 1)
  {
    ++stats.qcalls;
    value_type_t pivot = c.at((left + right)/2), tmp;
    while(i < j)
    {
      while(c.at(i) < pivot) i++;
      while(c.at(j) > pivot) j--;

      if(i < j)
      {
        //swap(c, i++, j--);
        tmp = c[i];
        c[i++] = c[j];
        c[j--] = tmp;
        //i++; j--;
        ++stats.qswaps;
      }
      if(left < j)  oqsort(c, left, j);
      if(i < right) oqsort(c, i, right);

    }
  }

  return c;
}

/**
 * qsort: executes quicksort algorithm selecting the midpoint of a container
 *        as a pivot value
 * @param Container c
 * @param size_t left
 * @param size_t right
 * @return
 */
template <class Container>
Container& qsort(Container & c, const size_t left, const size_t right)
{
  size_t i = left,
         j = right, mid, dif = j - i;

  if(dif > 1)
  {
    ++stats.qcalls;

    if(dif < M)
    {
      isort(c, i, j);
    }

    mid = ((i + j)/2);
    value_type_t pivot = max(max(c[mid], c[mid-1]), c[mid+1]), tmp;
    //value_type_t pivot = c[mid], tmp;

    while(i < j)
    {
      while(c[i] < pivot) i++;
      while(pivot < c[j]) j--;

      if(i < j)
      {
        tmp = c[i];
        c[i++] = c[j];
        c[j--] = tmp;
        ++stats.qswaps;
        if(left < j)  qsort(c, left, j);
      }
      if(i < right) qsort(c, i, right);
    }
  }

  return c;
}

/**
 * render:
 */
template <class Container>
void render(Container & c)
{
  cerr << endl << "Rendering..." << endl;

  for(size_t i = 0; i < c.size(); ++i)
  {
    cerr << "c[" << i << "]:" << "\t" << c.at(i) << endl;
  }

  cerr << "Rendering Complete" << endl << endl;
}

/**
 * generate:
 *
 * @return
 */
template <class Container>
Container& generate(Container & c, bool usestatic = 0)
{
  cout << endl << "Generating..." << endl;
  c.clear();
  for(size_t i = 0; i < n; ++i)
  {
    if(usestatic) c.insert(c.begin(), sav[i]);
    else c.insert(c.begin(), genrand());
  }

  render(c);

  cout << endl << "Generation Complete" << endl;

  return c;
}

/**
 *
 */
void statstr(stringstream & tss, Timer & t, int i)
{
  tss << setw(8) << setfill(' ') << setprecision(6) << right
      << i
      << setw(16) << setfill(' ') << setprecision(6) << right
      << t.getDurationSecs()
      << setw(9) << setfill(' ') << setprecision(6) << right
      << stats.icalls
      << setw(9) << setfill(' ') << setprecision(6) << right
      << stats.qcalls
      << setw(9) << setfill(' ') << setprecision(6) << right
      << stats.iswaps
      << setw(10) << setfill(' ') << setprecision(6) << right
      << stats.qswaps
      << setw(10) << setfill(' ') << setprecision(6) << right
      << stats.iswaps + stats.qswaps << endl;
}

/**
 *
 */
template <class Container>
Container& testisort(Container & c, stringstream & tss, int i)
{
  stats.init();
  ist.start();
  isort(c, 0, n - 1);
  ist.stop();
  isum += ist.getDurationSecs();
  cout << "Sorted\n";
  render(c);

  statstr(tss, ist, i);

  return c;
}

/**
 *
 */
template <class Container>
Container& testoisort(Container & c, stringstream & tss, int i)
{
  stats.init();
  oit.start();
  oisort(c, 0, n - 1);
  oit.stop();
  oisum += oit.getDurationSecs();
  cout << "Sorted\n";
  render(c);

  statstr(tss, oit, i);

  return c;
}

/**
 *
 */
template <class Container>
Container& testqsort(Container & c, stringstream & tss, int i)
{
  stats.init();
  qst.start();
  qsort(c, 0, n - 1);
  qst.stop();
  qsum += qst.getDurationSecs();
  cout << "Sorted\n";
  render(c);

  statstr(tss, qst, i);

  return c;
}

/**
 *
 */
template <class Container>
Container& testoqsort(Container & c, stringstream & tss, int i)
{
  stats.init();
  oqt.start();
  oqsort(c, 0, n - 1);
  oqt.stop();
  oqsum += oqt.getDurationSecs();
  cout << "Sorted\n";
  render(c);

  statstr(tss, oqt, i);

  return c;
}

/**
 *
 */
void clear()
{
  isv.clear();
  oiv.clear();
  qsv.clear();
  oqv.clear();
  ivec.clear();
}

/**
 * testsort:
 *
 * @return
 */
template <class Container>
Container& testsort(Container & c)
{
  stringstream tss;

  cout << endl << "Testing..." << endl;
  isum = oisum = qsum = oqsum = 0;
  for(int i = 0; i < cycles; ++i)
  {
    qsv = isv = oqv = oiv = generate(c);

    testisort(isv, tss, i);
    testoisort(oiv, tss, i);
    testoqsort(qsv, tss, i);
    testqsort(oqv, tss, i);

    clear();
  }

  double  iavg = isum/cycles ,  qavg = qsum/cycles,
         oiavg = oisum/cycles, oqavg = oqsum/cycles,
         iqdif = max(iavg, qavg)  - min(iavg, qavg),
         oqdif = max(qavg, oqavg) - min(qavg, oqavg);

  cout << header    << tss.str() << endl << endl
       << setprecision(std::numeric_limits< double >::digits10) << setfill('0')
       << "IAVG:  " << iavg      << endl
       << setprecision(std::numeric_limits< double >::digits10) << setfill('0')
       << "QAVG:  " << qavg      << endl
       << setprecision(std::numeric_limits< double >::digits10) << setfill('0')
       << "OIAVG: " << oiavg     << endl
       << setprecision(std::numeric_limits< double >::digits10) << setfill('0')
       << "OQAVG: " << oqavg     << endl
       << setprecision(std::numeric_limits< double >::digits10) << setfill('0')
       << "IQDIF: " << iqdif     << endl
       << setprecision(std::numeric_limits< double >::digits10) << setfill('0')
       << "OQDIF: " << oqdif     << endl
       << "Testing Complete"     << endl << endl;

  oqv.clear();

  return ivec;
}

/**
 * main:
 *
 */
int main(int argc, char** argv)
{
  cout << endl;
  if(argc == 4)
  {
    strtoval(argv[1], cycles);
    strtoval(argv[2], n);
    strtoval(argv[3], M);
  }
  else
  {
    cout << "Using defaults for cycles, n, and threshold" << endl
       << "To specify test config execute with the following:" << endl
       << "./<prog name> <cycles> <elements> <threshold>" << endl
       << "   cycles: the number of times the test should be performed" << endl
       << " elements: the number of elements to be sorted" << endl
       << "threshold: the number of elements when quicksort uses inserstionsort"
       << endl << endl;
  }

  srand(time(NULL));  //seed random number generator

  cout << "Test Sort" << endl
       << "   Cycles: " << cycles << endl
       << " Elements: " << n << endl
       << "Threshold: " << M << endl << endl;
  testsort(ivec);

  cout << "M: " << M << " N: " << n << endl;
  return 0;
}

