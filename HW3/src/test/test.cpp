/****************************************************************************
  FileName     [ test.cpp ]
  PackageName  [ test ]
  Synopsis     [ Test program for class ModNum ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <cstdlib>
#include "calcModNum.h"

using namespace std;

#define NUM_MOD   10

// Make it global variable...
ModNum num[NUM_MOD];

void reset()
{
   for (size_t i = 0; i < NUM_MOD; ++i)
      num[i] = 0;
}

void print()
{
   for (size_t i = 0; i < NUM_MOD; ++i)
      cout << "num[" << i << "] = " << num[i] << endl;
   cout << endl;
}

void run(int num1, int num2, int num3)
{
   num[0] = num1;
   num[1] = -num2;
   num[2] = num[0] + num[1];
   num[3] = num[0] - num[1];
   num[4] = num[0] * num[1];
   num[5] = num[2] + num3;
   num[6] = num[3] - num3;
   num[7] = num[4] * num3;

   print();

   num[0] += num[1];
   num[2] -= num[3];
   num[4] *= num[5];
   num[6] = (num[0] == num[1])? num[2] : 0;
   num[7] = (num[0] != num[1])? num[3] : 0;

   print();
}

int main(int argc, char **argv)
{
   if (argc != 4) {
      cerr << "Error: using testMC <int> <int> <int>!!" << endl;
      exit(-1);
   }

   int num1 = atoi(argv[1]);
   int num2 = atoi(argv[2]);
   int num3 = atoi(argv[3]);

   // Modulus = 16
   cout << "Setting modulus = 16..." << endl;
   ModNum::setModulus(16);
   run(num1, num2, num3);

   // reset...
   reset();

   // Modulus = 7
   cout << "Setting modulus = 7..." << endl;
   ModNum::setModulus(7);
   run(num1, num2, num3);
}
