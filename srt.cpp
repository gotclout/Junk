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
 *  values: indicates the number values or size of the container to be sorted
 * default: values = 9 unless user specified
 *
 * argv[3]
 *  thresh: indicates the value at which insertion sort may be more efficient
 *          for trivially small containers
 * default: -1, unused unless user specified
 *****************************************************************************/
static int cycles =  1;
static int values =  9;
static int thresh = -1;

/** ivec is the Container to be sorted **/
vector<int> ivec;

/** A string stream for manipulating string values **/
stringstream sstr;

/******************************************************************************
 * string string manipulation MACROS
 *
 * strtoval: converts string values to integer value or the type of v
 *   clsstr: initializes the string stream or clears previous data if sstr
 *           has been initialized
 *****************************************************************************/
#define strtoval(str, v) sstr << str; sstr >> v;
#define clsstr() sstr.str(""); sstr.clear();

/** MACRO to generate random values between 0 and values * 10 **/
#define genrand() rand() % values

/** Template for sorting any container with template typename **/
#define value_type_t typename Container::value_type

/**
 * isort: executes insertion sort algorithm
 *
 * @param Container c
 * @return
 */
template <class Container>
Container& isort(Container & c)
{
  size_t i  = 0,
         j  = 0,
         sz = c.size();

  int tmp;

  for( ; i < sz; ++i)
  {
    j = i;
    while(j > 0 && ivec[j-1] > ivec[j])
    {
      tmp = ivec[j];
      ivec[j] = ivec[j-1];
      ivec[j--] = tmp;
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
  size_t i = left, j = right;

  if(j - i > 1)
  {
    if(c.size() < 0) isort(c);
    else
    {
      value_type_t pivot = c.at((left + right)/2);

      while(i < j)
      {
        while(c.at(i) < pivot) i++;
        while(c.at(j) > pivot) j--;

        if(i < j)
        {
          value_type_t tmp = c.at(i);
          c.at(i++) = c.at(j);
          c.at(j--) = tmp;
        }

        if(left < j) qsort(c, left, j);
        if(i < right) qsort(c, i, right);
      }
    }
  }
  return c;
}

/**
 * populate:
 *
 * @return
 */
vector<int>& populate()
{
  return ivec;
}

/**
 * render:
 */
void render()
{
  cout << "Rendering..." << endl;

  for(size_t i = 0; i < ivec.size(); ++i)
  {

  }
}

/**
 * testsort:
 *
 * @return
 */
vector<int>& testsort()
{
  for(int i = 0; i < cycles; ++i)
  {
    ivec = populate();
    qsort(ivec, 0, values);
    render();
    ivec.clear();
  }

  return ivec;
}


/**
 * main:
 *
 */
int main(int argc, char** argv)
{
  if(argc == 1)
  {
    strtoval(argv[1], cycles);
  }
  else
  {

  }

  testsort();

  return 0;
}

