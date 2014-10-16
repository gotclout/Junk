#include <iostream>
#include <sstream>
#include <string>
#include <stdlib.h>

#define N 8

using namespace std;

int gvals[N];


template <typename K, typename V>
class heap
{
  private:

  class node
  {
    private:

    protected:

    public:

      K key;
      V value;
      node* parent;
      node* left;
      node* right;

      node()
      {
        parent = left = right = 0;
      };

      node(K pKey, V pValue)
      {
        key = pKey;
        value = pValue;
        parent = left = right = 0;
      };
      /*
         node(const node & n)
         {
         key = n.key;
         value = n.value;

         parent = n.parent;
         left = n.left;
         right = n.right;
         }

         node& operator=(const node & rhs)
         {
         if(this != & rhs)
         {
         key = rhs.key;
         value = rhs.value;

         parent = rhs.parent;
         left = rhs.left;
         right = rhs.right;
         }

         return * this;
         }
         */
      /**
       *
       */
      bool operator < (node* & rhs)
      {
        node n = *rhs;
        bool retVal = *this < n;
        return retVal;
      };

      bool operator < (const node & rhs){ return this->key < rhs.key; };
      bool operator < (K & rhs) { return key < rhs; };

      /**
       *
       */
      bool operator > (node* & rhs)
      {
        node n = *rhs;
        bool retVal = *this > n;
        return retVal;
      };

      bool operator > (const node & rhs){ return this->key > rhs.key; };
      bool operator > (K & rhs) { return key > rhs; };

      friend ostream& operator<<(ostream & o, node & n)
      {
        o << "[key: " << n.key << ", value: " << n.value
          << "]\n";

        if(n.parent)
          o << "parent: " << n.parent->key << endl;
        else o << "orphan node\n";

        if(n.left && n.right)
          o << "[left: " << n.left->key << ", right: " << n.right->key << "]\n";
        else if(n.left)
          o << "left child: " << n.left->key << '\n';
        else if(n.right)
          o << "right child: " << n.right->key << '\n';
        else
          o << "leaf node\n";

        return o;
      }
  };

  node* root, *helper;

  bool maxHeap;

  int nodes;

  protected:

  node* swapnodes(node* & src, node* & dst)
  {
    cout << "Swap Nodes: [SRC = " << src->key << ", DST = " << dst->key
      << "]\n";

    helper   = dst;
    node* p  = src->parent,
      *tmp = 0;

    if(src->right == dst)
    {
      src->right = helper->right;
      if(src->right)
        src->right->parent = src;
      helper->right = src;
      node* tmp = src->left;
      src->left = helper->left;
      if(src->left)
        src->left->parent = src;
      helper->left = tmp;
      if(helper->left)
        helper->left->parent = helper;
      src->parent = helper;
      helper->parent = p;
    }
    else if(src->left == dst)
    {
      src->left = helper->left;
      if(src->left)
        src->left->parent = src;
      helper->left = src;

      node* tmp = src->right;
      src->right = helper->right;
      if(src->right)
        src->right->parent = src;
      helper->right = tmp;
      if(helper->right)
        helper->right->parent = helper;
      src->parent = helper;
      helper->parent = p;
    }

    if(p && p->left == src)
      p->left = helper;
    else if(p && p->right == src)
      p->right = helper;

    if(!helper->parent)
      root = helper;

    return helper;
  }

  void siftup(node* & n)
  {
    if(n)
    {
      node* next = 0;
      if(n->left && n->right)
      {
        if(*n < n->left)
        {
          next = swapnodes(n, n->left);
          siftup(next->parent);
        }
        else if(*n < n->right)
        {
          next = swapnodes(n, n->right);
          siftup(next->parent);
        }
        /*else
          next = 0;
        if(next)
          siftup(next->parent);*/
      }
      else if(n->left && *n < n->left)
      {
        next = swapnodes(n, n->left);
        siftup(next->parent);
      }
      else if(n->right && *n < n->right)
      {
        next = swapnodes(n, n->right);
        siftup(next->parent);
      }
      /*else if(n->parent)
      {
        next = n->parent;
        siftup(next);
      }*/
      else
        return;
    }
  };

  public:

  heap(bool pMax = 1)
  {
    root = 0;
    maxHeap = pMax;
    nodes = 0;
  };

  void heapify(bool pMax = 1)
  {
    maxHeap = pMax;
    nodes = 0;
  };

  friend ostream& operator << (ostream & o, heap & h)
  {
    o << "Rendering Heap\n" << "Nodes: " << h.nodes << '\n';
    h.root ? o << *h.root << h.printtree(h.root) : o << "Heap is empty...\n";
    return o;
  }

  string _printtree(node* n)
  {
    stringstream ss;
    string s;

    ss << "";

    if(n)
    {
      if(n->left)
      {
        ss << n->left->value << " ";
      }
      if(n->right)
      {
        ss << n->right->value << " ";
      }
      ss << _printtree(n->left);
      ss << _printtree(n->right);
    }

    return ss.str();

  };

  string printtree(node* n)
  {
    stringstream ss;
    string s;

    ss << "";

    if(n)
    {
      if(n->left)
      {
        ss << *n->left;
      }
      if(n->right)
      {
        ss << *n->right;
      }
      ss << printtree(n->left);
      ss << printtree(n->right);
    }

    return ss.str();
  }

  bool insert(const K & key, const V & value)
  {
    bool retVal = true;
    node* n = new node(key, value);
    n->parent = 0;
    if(!root)
    {
      root = n;
      cout << "Insert Root: " << key << '\n';
    }
    else
    {
      node* cur = root;
      while(cur)
      {
        if(cur->key == key)
        {
          delete n;
          cur = 0;
        }
        else if(!cur->right && !cur->left)
        {
          n->parent = cur;
          cur->left = n;
          siftup(cur);
          cur = 0;
        }
        else if(!cur->right && cur->left)
        {
          cur->right = n;
          n->parent = cur;
          siftup(cur);
          cur = 0;
        }
        else if(!cur->left && cur->right)
        {
          cur->left = n;
          n->parent = cur;
          siftup(cur);
          cur = 0;
        }
        else if(cur->left && cur->right)
        {
          if(cur->parent && (!cur->parent->right->left ||
                !cur->parent->right->right))
          {
            if(!cur->parent->right->left)
            {
              cur = cur->parent->right;
              cur->left = n;
            }
            else if(!cur->parent->right->right)
            {
              cur = cur->parent->right;
              cur->right = n;
            }
            n->parent = cur;
            siftup(cur);
            cur = 0;
          }
          else
            cur = cur->left;
        }
      }
    }

    if(retVal)
      nodes++;

    return retVal;
  };

  node* pop()
  {
    node* top = 0;

    if(nodes)
    {
      top = root;

      node* max = 0;

      node* next = 0;
      if(root->left && root->right)
      {
        if(*root->left > *root->right)
        {
          max = root->left;
          next = root->right;
        }
        else
        {
          max = root->right;
          next = root->left;
        }

        K key = max->key;
        V val = max->value;

        node*tmp = next;
        while(tmp->left || tmp->right)
          tmp = tmp->right;

        tmp->left = max->left;
        tmp->left->parent = tmp;
        tmp->right = max->right;
        tmp->right->parent = tmp;
        root = next;
        next->parent = 0;
        nodes -= 2;
        insert(key, val);
      }
      else if(root->left)
      {

      }
      else if(root->right)
      {

      }
      else
      {

      }
    }

    return top;
  };

  void merge()
  {

  };

  void peek()
  {
    if(nodes > 0)
    {
      cout << root;
    }
  }

    const int size() { return nodes; };
  };


  int main(int argc, char* argv[])
  {

    heap<int, long> h;

    srand(time(0));

    int k;
    long v;
    gvals[0] = 6;
    gvals[1] = 5;
    gvals[2] = 3;
    gvals[3] = 1;
    gvals[4] = 8;
    gvals[5] = 7;
    gvals[6] = 2;
    gvals[7] = 4;
    for(int i = 0; i < N; i++)
    {
      //k = rand() % 20 + 1;
      k = gvals[i];
      v = rand() % 40 + 1;

      h.insert(k, v);
    }

    cout << h;

    h.pop();

    return 0;
  }
