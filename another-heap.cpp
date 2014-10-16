#include <iostream>
#include <string>

using namespace std;

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
      };
  };

  node* root;

  protected:

  void sift()
  {
    if(root)
    {
      node* tmp = root;


    }
  }
  public:

  heap()
  {
    root = 0;
  };

  void heapify()
  {

  };

  bool insert(const K & key, const V & value)
  {
    bool retVal = true;
    node* n = new node(key, value);
    if(!root)
    {
      root = n;
      n->parent = 0;
    }
    else if(!root->left && key < root->key)
    {
      root->left = n;
      n->parent = root;
    }
    else if(!root->right && key > root->key)
    {
      root->right = n;
      n->parent = root;
    }
    else
    {
      node* tmp = root;

      while(tmp)
      {
        if(tmp->left && tmp->right)
        {
          if(key < tmp->left->key)
          {
            tmp = tmp->left;
          }
          else
          {
            tmp = tmp->right;
          }
        }
        else if(!tmp->left && !tmp->right)
        {
          if(key < tmp->key)
          {
            tmp->right = n;
          }
          else
          {
            tmp->left = n;
          }

          n->parent = tmp;
          tmp = 0;
        }
        else if(tmp->left)
        {
          if(key < tmp->left->key)
          {
            tmp->right = tmp->left;
            tmp->left = n;
          }
          else
            tmp->right = n;

          n->parent = tmp;
          tmp = 0;
        }
        else if(tmp->right)
        {
          if(key < tmp->right->key)
          {
            tmp->left = tmp->right;
            tmp->right = n;
          }
          else
            tmp->left = n;

          n->parent = tmp;
          tmp = 0;
        }
      }
    }

    return retVal;
  };

  void remove()
  {

  };

  void merge()
  {

  };
};


int main(int argc, char* argv[])
{

  return 0;
}
