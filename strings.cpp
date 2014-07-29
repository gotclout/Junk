#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

const static string str     = "The quick brown fox jumps swiftly over the lazy brown dog at night\n";
const static string uvowels = "ieayiiieaaaouyeeeeyayayayaiiiaaaeooouuuuuuuuaeiouy";
const static string vowels  = "aeiouy";

map<char, string> vmap;
map<char, string>::iterator vmapIt;

stringstream sstr;

/**
 * Display the first and last segments of str containing s
 */
void segment(const string & s)
{
  size_t pos = str.find_first_of(s);
  string val = str.substr(0, pos + s.size());
  cout << "Start Segment: " << val << '\n';
  pos = str.find_last_of(s);
  val = str.substr(pos, str.size() - pos);
  pos = str.rfind(s);
  val = str.substr(pos, str.size() - pos);
  cout << "  End Segment: " << val << '\n';
}

/**
 * Sort the vowels in uvowels into bins
 */
string sortvowels()
{
  string retVal = "    Vowel Map: ";
  int i = 0;
  for(i; i < vowels.size(); ++i) vmap[vowels[i]] = "";

  for(i = 0; i < uvowels.size(); ++i) vmap[uvowels[i]] += uvowels[i];

  for(i = 0; i < vowels.size(); ++i)
  {
    retVal += vmap[vowels[i]];
    if(vmap[vowels[i]] != vmap[vowels[i+1]]) retVal += "\n               ";
  }
  return retVal;
}

/**
 * Print the specified vowel in vmap
 */
string printavowel(const char & c)
{
  string retVal = "";

  for(vmapIt = vmap.begin(); vmapIt != vmap.end(); ++vmapIt)
  {
    if(vmapIt->first == c)
    {
      retVal += vmapIt->second + "\n";
      break;
    }
  }

  return retVal;
}

/**
 * Find string s in str
 */
size_t find(const string & s)
{
  sstr << "   Search Str: " <<  s << "\n   Source Str: " <<  str
       << "     Position: ";
  size_t pos = str.find(s);
  sstr << pos;
  if(pos != string::npos)
  {
    cout << sstr.str() + "\n";
    sstr.str("");
    sstr.clear();
  }
  else cout << sstr.str() << "DNE\n";

  return pos;
}

/**
 * Find and replace the string s with r in str
 */
string replace(const string & s, const string & r)
{
  string tmp = str;
  cout << "\n  Replace Str: " << r << '\n';
  size_t pos = find(s);

  if(pos != string::npos)
  {
    tmp = tmp.replace(pos, s.size(), r);
    cout << "   Result Str: " <<  tmp;
  }
  
  return tmp;
}

/**
 * Execute string functions
 */
int main(int argc, char** argv)
{
  const string s = "brown",
               r = "orange";

  cout << "      Segment:\n";
  segment(s);
  cout << "         Find:\n";
  find(s);
  cout << "\n      Replace:";
  replace(s, r);
  cout << "\n         Sort:\n" << sortvowels() << "\n        Print:\n       Vowels: ";
  string (*pfuncptr) (const char &) = &printavowel;
  cout << (*pfuncptr) ('a');
  for(int i = 1; i < 6; ++i) cout << "               " <<  printavowel(vowels[i]);
  cout << "\nfin\n";

  return 0;
}
