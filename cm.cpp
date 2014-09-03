#ifndef __MCRYPTO__
#define __MCRYPTO__

#include <iostream>
#include <sstream>
#include <climits>
#include <ostream>
#include <iomanip>
#include <cmath>

#define B10216 a[i] - (a[i] / 10) + a[i] % 10
using namespace std;

static const char B64 [] =
 "ABCDEFGHIJKLMNOP"\
 "QRSTUVWXYZabcdef"\
 "ghijklmnopqrstuv"\
 "wxyz0123456789+\\";

class MCrypto
{
  private:

  protected:

  public:

  /**
   *
   */
  MCrypto()
  {

  };

  /**
   *
   */
  ~MCrypto()
  {

  };

  //ascii  0-9 b10  48 - 57   b16 30 - 39 b64 52 - 61
  //ascii  A-Z b10  65 - 90   b16 41 - 5A b64 00 - 25
  //ascii  a-z b10  97 - 122  b16 61 - 7A b64 26 - 51

  /**
   *
   */
  string ascii_to_b16(const string & s)
  {
    stringstream ss;
    ss << hex ;//<< setfill('0');

    for(size_t i = 0; i < s.length(); ++i)
      ss << s[i];
      //ss << setw(2) << s[i];

    return ss.str();
  };

  /**
   *
   */
  char* ascii_to_b16(const char* s, size_t & sz)
  {
    stringstream ss;
    stringstream zz;
    char* buf = new char[sz];
    memset(buf, 0, sz);
    for(size_t i = 0; i < sz; ++i)
    {
      ss << hex << s[i];
      ss >> buf[i];
      ss.str("");
      ss.clear();
    }
    sz *= sizeof(char);
    return buf;
  };

  /**
   *
   */
  char* ascii_to_b64(const string & s, size_t & sz)
  {
    stringstream ss;
    ss << ios::binary;
    size_t i = 0;
    for( ; i < s.length(); ++i)
    {
      char c = s[i];
      if(c <= 57 && c >= 48) c += 4;
      else if(c <= 90 && c >= 65) c -= 65;
      else if(c <= 122 && c >= 97) c -= 71;
      ss.write(&c, sizeof(unsigned int));
    }
    sz = --i;
    sz *= sizeof(char*);
    char* buf = new char[sz];
    memset(buf, 0, sz);
    ss >> buf;
    return buf;
  };

  /**
   *
   */
  int ascii_to_b64(int in)
  {
    stringstream sst;
    sst << hex << setw(2) << setfill('0');
    sst << in;
    cout << "sst: " << sst.str() << endl ;
    int c;
    sst >> c;
    if(c <= 57 && c >= 48) return c - (c  - '0') - 18 + 12;
    else if(c <= 90 && c >= 65) return c - (c - 'A') - 24 - 41;
    else if(c <= 122 && c >= 97) return c - (c - 'a') - 36 - 35;
    else return 63;
  ;}


  /**
   *
   */
  void* b16_to_b64(const char* s, size_t & sz)
  {
    stringstream ss;
    stringstream zz;
    size_t offset = 0;
    while( (sz * sizeof(int) + offset) % 8) offset += 2;
    unsigned int* ai = new unsigned int[sz];
    memset((void*)ai, 0, sz * sizeof(int));
    for(size_t i = 0; i < sz; ++i)
    {
      unsigned int ui = (unsigned int) s[i];
      zz << hex << setw(2) << setfill('0') << ui;
      zz >> ui;
      //ss << ui;
      //ss >> ai[i];
      ai[i] = ui;
      zz.str("");
      zz.clear();
    }
    sz *= sizeof(int);
    return ai;
  };

  /**
   *
   */
  void print_b64(const void* s, size_t sz)
  {

    stringstream ss;
    ss << "Render B64...\n" << "\nlen: " << sz << "\nstr: " << (char*)s << '\n';

    int    b012345 = 0,
           b67     = 0,
           b670123 = 0,
           b4567   = 0,
           b456701 = 0,
           b234567 = 0;

    int*   a       = (int*) s;

    size_t szup    = ((sz/64) + 1) * 64,
           intsz   = sizeof(int),
           bnd     = sz/intsz;

    bnd = bnd + bnd % intsz;

    for(size_t i = 0, v = 0; i < bnd; ++i, v = i%4)
    {
      if(i*intsz >= sz) ss << '=';
      else if((i == 0) || (i % 4 == 0))
      {
        // end of 8 bit block or next byte
        int pos = a[i];
        if(i > 0) pos = a[i-1];
        b012345 = pos >> 2;
        ss << B64[b012345];
      }
      else if(v == 1)
      {
        b67     = a[i-1] ^ ((a[i-1] >> 2) << 2);
        b670123 = ( (b67 << 4) | (a[i] >> 4) );
        ss << B64[b670123];
      }
      else if(v == 2)
      {
        b4567   = a[i-1] ^ ((a[i-1] >> 4) << 4);
        b456701 = (b4567 << 2) | (a[i] >> 6);
        ss << B64[b456701];
      }
      else// (v == 3)
      {
        b234567 = a[i-1] ^ ((a[i-1] >> 6) << 6);
        //b012345 = a[i] >> 2;
        //ss << B64[b012345];
        ss << B64[b234567];
      }
    }

    cout << ss.str();
    ss.str("");
    ss.clear();
  };

  /**
   *
   */
  int b16_to_b64(char in)
  {
    int out;
    if(in <= '9' && in >= '0') { out = in - 18; }
    else if(in <= 'Z' && in >= 'A') { out = in - 24; }
    else if(in <= 'z' && in >= 'a') { out = in - 36; }
    else if(out < '0' ) out = 62;
    else out = 63;
    return out;
  };

  int _b16_to_b64(char hex)
  {
    if ('0' <= hex && hex <= '9') {
      return hex - '0';
    }

    if (hex >= 'A' && hex <= 'Z') {
      return hex - 'A'  + 10;
    }

    return hex - 'a' + 10;
  };
};

#endif//__MCRYPTO__

