#include <iostream>

using namespace std;

class foo
{
  private:

  int a;

  protected:

  int b;

  public:

  int c;

    foo(){ a = 0; b = 0; c = 0;};
    virtual ~foo() {;};
    virtual void dostuff() = 0;
    virtual void domorestuff(){ a += 1; b+= a + 1; c += b + 1;};
    virtual void tostring(){cout << "a: " << a << "\nb: " << b << "\nc: " << c << endl;};
};

class bar : protected foo
{
  private:

  int d;
  protected:
  int e;
  void fin() {cout << "fin!\n";};
  public:

    bar(){d = 4;};
    ~bar() {;};
    virtual void dostuff() {e = 0;};
    virtual void domorestuff(){ foo::domorestuff(); e += d + 1;};
    virtual void tostring() {foo::tostring();cout << "d: " << d << "\ne: " << e <<  endl;};
};

class baz : private bar
{
  private:

  public:

  int f;

  baz() {f = 0;};
  ~baz() {;};
  virtual void dostuff() {f++;};
  virtual void domorestuff() {bar::domorestuff(); f += bar::e;};
  virtual void tostring(){bar::tostring(); cout << "f: " << f << endl;};
  void fin(){bar::fin();};
};

int main(char** argv, int argc)
{
  baz* bz = new baz();
  bz->dostuff();
  bz->domorestuff();
  bz->tostring();
  bz->fin();
  //delete bz;
  return 0;
}
