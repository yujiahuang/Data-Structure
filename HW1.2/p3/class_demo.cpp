
#include <iostream>
#include <iomanip>

using namespace std;

class A
{
public:
   A(int i = 0):_data(i) {}
   virtual ~A() {}

   virtual int operator ()(int i) const = 0;

protected:
   int  _data;
};

class C1 : public A
{
public:
   C1(int i = 0):A(i) {}
   virtual int operator ()(int i) const { return (_data + i); }
};

class C2 : public A
{
public:
   C2(int i = 0):A(i) {}
   virtual int operator ()(int i) const { return (_data - i); }
};

void f(const A& a, int i)
{
   cout << a(i) << endl;
}

int main()
{
   f(C1(10), 15);
   f(C2(10), 15);
}
