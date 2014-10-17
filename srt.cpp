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

/** Open Source Timer Impl                                                   **/
#include "Timer.h"
Timer t;

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
 * default: 0, unused unless user specified
 ******************************************************************************/
static int cycles = 1;//1000;
static int n      = 31;
static int M      = 10;

/** ivec is the Container to be sorted                                       **/
typedef vector<int> ivt;
static ivt ivec;

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
  size_t i  = beg,
         j  = 0,
         sz = end - beg;

  for( ; i <= sz; ++i)
  {
    value_type_t tmp = c.at(i);
    j = i;
    while(j > beg && c.at(j-1) > tmp)
    {
      c.at(j) = c.at(--j);
      ++stats.iswaps;
    }
    c.at(j) = tmp;
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
         j = right;
  int  dif = (int) j - i;

  ++stats.qcalls;
  if(dif > 1)
  {
    //++stats.qcalls;
    if(dif < M && dif + 1 != n)
    {
      return isort(c, i, j);
      ++stats.icalls;
    }
    else
    {
      value_type_t pivot = c.at((left + right)/2);
      while(i < j)
      {
        while(c.at(i) < pivot) i++;
        while(c.at(j) > pivot) j--;

        if(i < j)
        {
          swap(c, i++, j--);
          ++stats.qswaps;
        }

        if( left < j  )  qsort(c, left, j);
        if( i < right )  qsort(c, i, right);
      }
    }
  }

  return c;
}

/**
 * render:
 */
void render()
{
  cerr << endl << "Rendering..." << endl;

  for(size_t i = 0; i < ivec.size(); ++i)
  {
    cerr << "ivec[" << i << "]:" << "\t" << ivec.at(i) << endl;
  }

  cerr << "Rendering Complete" << endl << endl;
}

/**
 * generate:
 *
 * @return
 */
vector<int>& generate()
{
  cout << endl << "Generating..." << endl;
  for(size_t i = 0; i < n; ++i)
  {
    ivec.insert(ivec.begin(), genrand());
  }

  render();

  cout << endl << "Generation Complete" << endl;

  return ivec;
}

/**
 * testsort:
 *
 * @return
 */
vector<int>& testsort()
{
  stringstream tss;
  ivt v;

  cout << endl << "Testing..." << endl;
  for(int i = 0; i < cycles; ++i)
  {
    stats.init();
    v = generate();
    ivec = v;
    render();
    t.start();
    // decrement end by 1 for containers indexed at 0
    qsort(ivec, 0, n - 1);
    t.stop();
    render();

    tss << setw(8) << setfill(' ') << setprecision(6)
        << i               << "        "  << t.getDurationSecs() << "   "
        << stats.icalls    << "        "  << stats.qcalls        << "    "
        << stats.iswaps    << "         " << stats.qswaps        << endl;

    ivec.clear();
    v.clear();
  }
  cout << header << tss.str() << endl << endl
       << "Testing Complete"  << endl << endl;

  return ivec;
}


/**
 * main:
 *
 */
int main(int argc, char** argv)
{
  cout << endl;
  if(argc == 3)
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
         << "threshold: the number of elements until quick sort defaults to inserstion sort"
         << endl << endl;
  }

  srand(time(NULL));  //seed random number generator

  cout << "Test Quick Sort" << endl
       << "   Cycles: " << cycles << endl
       << " Elements: " << n << endl
       << "Threshold: " << M << endl << endl;
  testsort();

  return 0;
}

