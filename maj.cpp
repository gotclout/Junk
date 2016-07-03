#include <iostream>
#include <vector>
#include <map>

using namespace std;

const int maxelem = 23; //num elements to generate

/**
 * Find majority or max occurs of values in elements 
 * elemements not empty
 */
int majority(vector<int> & e)
{
  int m  = 0;        //majority value
  map<int, int> kv;  //map key element value occurrences

  for(int i = 0; i < maxelem; ++i)
  {
    kv[e[i]] += 1;
    if(kv[e[i]] > m) m  = e[i];
  }

  //print
  cout << "map:\n";
  int sz = maxelem/2, j = 0;
  for( ; j < sz; ++j) cout << j << " -> " << kv[j] << "\n";

  return m;
}

int main(int argc, char** argv)
{
  srand(time(0));
  vector<int> v;

  //generate
  for(int i = 0; i < maxelem; ++i) v.push_back(rand() % (maxelem/2));

  //print
  cout << "elems: ";
  for(int j = 0; j < maxelem; ++j) cout << v[j] << " ";
  cout << "\n";

  //compute majority
  int maj = majority(v);
  cout << "Majority Element: " << maj << "\n";
  
  return 0;
}
