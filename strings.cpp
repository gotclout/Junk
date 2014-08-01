#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>

#define align cout << setw(16) << setfill(' ')

using namespace std;

const static string str     =
  "The quick brown fox jumps swiftly over the lazy brown dog at night";
const static string uvowels =
  "ieayiiieaaaouyeeeeyayayayaiiiaaaeooouuuuuuuuaeiouy";
const static string vowels  = "aeiouy";

map<char, string> v;
map<char, string>::iterator vIt;


/**
 * Display the first and last segments of str containing s
 */
void segment(const string & s)
{
  size_t pos = str.find_first_of(s);
  string val = str.substr(0, pos + s.size());
  align << right << "Start Segment: " << val << '\n';
  pos = str.find_last_of(s);
  val = str.substr(pos, str.size() - pos);
  pos = str.rfind(s);
  val = str.substr(pos, str.size() - pos);
  align << right << "End Segment: " << val << endl << endl;
}

/**
 * Sort the vowels in uvowels into bins
 */
string sortvowels()
{
  align << right << "Vowel Map: " << endl;
  for(size_t i = 0; i < uvowels.size(); ++i) v[uvowels[i]] += uvowels[i];
  for(size_t i = 0; i < vowels.size(); ++i)
  {
    if(v[vowels[i]] != v[vowels[i+1]])
      align << left << ' ' << v[vowels[i]] << '\n';
  }

  return "";
}

/**
 * Print the specified vowel in v
 */
string printvowel(const char & c)
{
  for(vIt = v.begin(); vIt != v.end(); ++vIt)
    if(vIt->first == c) return vIt->second;
  stringstream sstr;
  sstr << c << " is not a vowel\n";
  return sstr.str();
}

/**
 * Find string s in str
 */
size_t find(const string & s, string sstr = str)
{
  align << right << endl << "Search Str: " << s << endl;
  size_t pos = sstr.find(s);
  align << right << "Source Str: " << sstr << endl;
  if(pos == string::npos) align << right << "Position: " << "DNE" << endl;
  else align << right << "Position: " << pos << endl;

  return pos;
}

/**
 * Find and replace the string s with r in str
 */
string replace(const string & s, const string & r, string sstr = str)
{
  align << right << "Replace Str: " << r;
  string tmp = sstr;
  size_t pos = find(s, tmp);
  if(pos != string::npos) tmp = tmp.replace(pos, s.size(), r);
  align << right << "Result Str: " << tmp << endl << endl;

  return tmp;
}

/**
 * Find and replace all instances of s with r in str
 */
string replaceall(const string & s, const string & r, string sstr = str)
{
  align << right << "Replace All: " << endl;
  align << right << "Replace Str: " << r;
  string tmp = sstr;
  size_t pos = find(s, tmp);
  while(pos != string::npos)
  {
    tmp = tmp.replace(pos, s.size(), r);
    pos = find(s, tmp);
  }
  align << right << "Result Str: " << tmp << endl << endl;

  return tmp;
}

/**
 * Execute string functions
 */
int main(int argc, char** argv)
{
  const string s = "brown", r = "orange", e = "yellow";
  size_t sz = vowels.size();

  align << right << "Segment: " << endl;
  segment(s);
  align << right << "Find: ";
  find(e);
  find(s);
  align << right << endl << "Replace: " << endl;
  string rs = replace(s, r);
  rs = replace(s, r, rs);
  replace(s, r, rs);
  replaceall(s, r);
  align << right << "Sort: " << endl;
  align << right << sortvowels() << endl;
  align << "Print Vowels: " << endl;
  string (*pfuncptr) (const char &) = &printvowel;
  align << left << ' ' << (*pfuncptr) ('a') << endl;
  for(int i = 1; i < sz; ++i)
    align << left << ' ' << printvowel(vowels[i]) << endl;
  align << left << ' ' << printvowel('z') << "\nfin\n";

  return 0;
}
