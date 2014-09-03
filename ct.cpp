#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "MCrypto.h"

#define align cout << right << setw(12) << setfill(' ')

using namespace std;

MCrypto mc;

const string digits = "Man";

const string instr =
"49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f75\
3206d757368726f6f6d";

const string outstr =
  "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

bool convert()
{
  align << "convert : " << instr << endl;
  int b64[64];
  size_t len = instr.length(),
         i,
         j;

  for(i = 0; i < len; ++i)
    b64[i] = mc.b16_to_b64(instr[i]);

  int* p = b64;
  len = 64 * sizeof(int);
  char str[len];
  memset(str, 0, len);
  for(j = 0; j < len; ++p, ++j)
  {
    str[j] += mc.b64_to_b16(b64[j]);
  }

  string r(str);
  return r == outstr;
}

int main(int argc, char** argv)
{
  stringstream ss;

  size_t i;
  for(i = 0; i < digits.length(); ++i)
  {
    ss << "b16[" << i << "] : ";
    align << ss.str();
    ss.str("");
    ss.clear();
    cout << "b64[" << mc.b16_to_b64(digits[i]) << ']' << endl;
    cout << "b64[" << mc._b16_to_b64(digits[i]) << ']' << endl;
  }

  align << "instrlen :" << instr.length() << endl;
  align << "outstrlen :" << outstr.length() << endl;
  align << "size : " << sizeof(int) << endl;
  if(!convert())
    cerr << "ERROR: conversion failed" << endl;
  return 0;
}
