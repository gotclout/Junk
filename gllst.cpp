#include <iostream>
#include "timer.h"

using namespace std;

//Let N be a constant value for length
#define N 23

//Let gvals be a global array of N integers
int gvals[N];

/**
 * Nodes of a linked list
 */
struct lnode
{
  void* data;
  lnode* next;

  lnode() { data = 0; next = 0; };
};

/**
 * A linked list of nodes from head to tail
 */
struct list
{
  lnode* head;
  lnode* tail;

  /**
   * Default Constructor initializes list nodes to null
   */
  list() { head = tail = 0; };

  /**
   * List insertion, allocates memory for the size of data to be
   * stored in the list.
   *
   * @param void* data, is the data to be stored in the list lode
   * @param int _sz, is the size (bytes) of the data to be stored
   */
  bool insert(void* data, int size)
  {
    bool retVal = true;

    if(data)
    {
      lnode* n = new lnode();
      n->data = (void*) new char[size];
      memset(n->data, 0, size);
      memcpy(n->data, data, size);
      if(!head)
        head = tail = n;
      else
      {
        tail->next = n;
        tail = n;
      }
    }
    else
      retVal = false;

    return retVal;
  };

  /**
   * List Destructor
   *
   * Deallocates memory allocated by calling insert
   */
  ~list()
  {
    lnode* curr = head, *tmp = 0;
    while(curr)
    {
      if(curr->data)
      {
        char* d = (char*) curr->data;
        delete [] d;
        curr->data = 0;
      }
      tmp = curr;
      curr = curr->next;
      delete tmp;
      tmp = 0;
    }
  };
};

/**
 * Populate the gvals array with random values between 1 and N
 */
void initvals()
{
    for(int i = 0; i < N; i++)
      gvals[i] = rand() % N + 1;
}

/**
 * Test the list by creating, insert values, print values, and destroying.
 */
void testlist()
{
  list* l = new list();
  for(int i = 0; i < N; i++)
    l->insert(&gvals[i], sizeof(int));

  lnode* curr = l->head;

  while(curr)
  {
    cout << *((int*)(curr->data)) << endl;
    curr = curr->next;
  }

  delete l;
}

/**
 *
 */
bool cyclic(list* & l)
{
  bool rval = false;

  if(l)
  {
    lnode* cur = l->head;

    set<lnode*> discovered;

    while(cur && !rval)
    {   
      if(discovered.empty())
      {   
        discovered.insert(cur);
      }   
      else
      {   
        if(cur->next && cur->next == cur)
        {   
          rval = true;
        }   
        else if(discovered.find(cur) == discovered.end())
        {   
          discovered.insert(cur);
        }   
        else
        {   
          rval = true;
        }   
      }   
      cur = cur->next;
    }   
  }

  return rval;
}

/**
 * Seed the random number generator;
 * Generate a set of global integers
 * Test the list
 *
 * @param int argc, num args
 * @param char** argv, arg vector
 */
int main(int argc, char** argv)
{
  timer t;

  t.Start();

  srand(time(0));
  initvals();
  testlist();

  t.Stop();

  cout << "Time: " << t.Duration() << endl;
  return 0;
}
