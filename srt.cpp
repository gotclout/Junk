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

#include "Mutex.h"

static Mutex m;

/** no std::nonsense **/
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
 *  elems: indicates the number elems or size of the container to be sorted
 * default: elems = 9 unless user specified
 *
 * argv[3]
 *  thresh: indicates the value at which insertion sort may be more efficient
 *          for trivially small containers
 * default: -1, unused unless user specified
 ******************************************************************************/
static int cycles =  1;
static int elems =  9;
static int thresh = -1;

/** ivec is the Container to be sorted **/
static vector<int> ivec;

/** A string stream for manipulating string values                           **/
stringstream sstr;

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
#define genrand() rand() % (elems * 10)

/**     MACRO template for sorting any container with template typename     **/
#define value_type_t typename Container::value_type

/**
 * @sstats: a performance statistics object for analysis
 */
struct sstats
{
  int iswaps,
      qswaps,
      icalls,
      qcalls;

  /**
   * Def construct
   */
  sstats() { iswaps = qswaps = icalls = qcalls = 0; } ;

  /**
   * Output operator overload
   */
  friend ostream& operator <<(ostream & o, sstats & s)
  {
    o << "Rendering Current Statistics..."  << endl
      << "insertion sort calls: " << s.icalls << endl
      << " swaps to completion: " << s.iswaps << endl
      << "    quick sort calls: " << s.qcalls << endl
      << " swaps to completion: " << s.qswaps << endl;

    return o;
  };
};

/** Performance Statistics **/
static sstats stats;

/**
 * swap: replaces the content of values in a container
 *
 * @param
 * @param
 * @return
 */
template <class Container>
Container swap(Container & p, const size_t lval, const size_t rval)
{
  Container c = p;
  if(lval < c.size() && rval < c.size())
  {
    value_type_t tmp = c.at(lval);
    c.at(lval)       = c.at(rval);
    c.at(rval)       = tmp;
  }

  p = c;
  return c;
}


/**
 * isort: executes insertion sort algorithm
 *
 * @param Container c
 * @param
 * @param
 *
 * @return
 */
template <class Container>
Container isort(Container & c, const size_t beg, const size_t end)
{
  size_t i  = beg,
         j  = 0,
         sz = end - beg;

  int tmp;

  for( ; i < sz; ++i)
  {
    j = i;
    while(j > beg && c.at(j-1) > c.at(j))
    {
      c = swap(c, j, j-1);
      ++stats.iswaps;
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
Container qsort(Container & c, const size_t left, const size_t right)
{
  size_t i = left,
         j = right;
  int  dif = (int) j - i;

  //++stats.qcalls;
  if(dif > 1)
  {
     ++stats.qcalls;
    if(dif < thresh)
    {
      isort(c, i, j);
      ++stats.icalls;
    }
    else
    {
      value_type_t pivot = c.at((left + right)/2);
      while(i < j)
      {
        while(c.at(i) <= pivot && i < elems) i++;
        while(c.at(j) >= pivot && j != 0) j--;

        if(i < j)
        {
          swap(c, j, i);
          ++stats.qswaps;
        }

        if( i < right )  qsort(c, i, right);
        if( j > left  )  qsort(c, left, j);
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
  cout << "Rendering..." << endl;

  for(size_t i = 0; i < ivec.size(); ++i)
  {
    cout << "ivec[" << i << "]:" << "\t" << ivec.at(i) << endl;
  }

  cout << "Rendering Complete" << endl << endl;
}

/**
 * populate:
 *
 * @return
 */
vector<int>& populate()
{
  cout << "Generating..." << endl;
  for(size_t i = 0; i < elems; ++i)
  {
    ivec.insert(ivec.begin(), genrand());
  }

  render();

  cout << "Generation Complete" << endl;

  return ivec;
}

/**
 * testsort:
 *
 * @return
 */
vector<int>& testsort()
{
  cout << "Testing..." << endl;
  for(int i = 0; i < cycles; ++i)
  {
    ivec = populate();
    qsort(ivec, 0, elems - 1);
    render();
    ivec.clear();
  }

  cout << stats << endl << "Testing Complete" << endl << endl;

  return ivec;
}


/**
 * main:
 *
 */
int main(int argc, char** argv)
{
  if(argc == 3)
  {
    strtoval(argv[1], cycles);
    strtoval(argv[2], elems);
    strtoval(argv[3], thresh);
  }
  else
  {
    cout << "Using defaults for cycles, elems, and threshold" << endl
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
       << " Elements: " << elems << endl
       << "Threshold: " << thresh << endl;
  testsort();

  return 0;
}

