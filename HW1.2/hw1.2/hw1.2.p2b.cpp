/****************************************************************************
  FileName     [ hw1.2.p2b.cpp ]
  PackageName  [ HW1.2 ]
  Synopsis     [ For problem 2(b) of HW1.2 in DSnP class ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2011-2012 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

/************************************************/
/*    Static global variables and functions     */
/************************************************/
// NOTE-NOTE-NOTE-NOTE
// ==> Copy these functions and classes from hw1.2.p2a.cpp
//
// Original: XOBCD AFGHU JKLMN IPQRS TEVWYZ
// Mapped  : ABCDE FGHIJ KLMNO PQRST UVWXYZ
// mp[Original] = Mapped alphabet
// e.g. mp[X] = A
static string mp; // TODO

// xMap(Original) ==> Mapped alphabet
// Need to convert everything Upper-case
// return original character if it is not an alphabet
static char xMap(char c)
{
   // TODO
   return c;
}

/***********************************/
/*           class xStr            */
/***********************************/
//
// Take an original string, covert it to the mapped string
class xStr
{
public:
   xStr(){}
   xStr(const string& s): _str(s) {
      // TODO
   }

   size_t size() const { return 0; } // TODO
   bool operator < (const xStr& s) const { return true; } // TODO
   bool operator == (const xStr& s) const { return true; } // TODO

   friend ostream& operator << (ostream& os, const xStr& s);

private:
   string _str; // converted string
};

ostream& operator << (ostream& os, const xStr& s)
{
   // TODO
   return os;
}


/***********************************/
/*           class Book            */
/***********************************/
//
class Book
{
friend class Cmp;
public:
   Book(const string& s); // TODO

   // Compare by Crazy Doc's rules
   bool operator < (const Book& b) const; // TODO

   // Print out the original string "_str"
   friend ostream& operator << (ostream& os, const Book& b); // TODO

private:
   string  _str;     // original string
   xStr    _author;  // converted author name
   xStr    _book;    // converted book name
   int     _year;
};

Book::Book(const string& s) : _str(s)
{
   // TODO
}

bool
Book::operator < (const Book& b) const
{
   // TODO
   return true;
}

ostream& operator << (ostream& os, const Book& b)
{
   // TODO
   return os;
}

/***********************************/
/*             main()              */
/***********************************/
int main()
{
   ifstream ifile("hw1.2.p2b.in");
   ofstream ofile("hw1.2.p2b.out");

   char buf[101];
   ifile.getline(buf, 100);
   int n = atoi(buf);

   vector<Book> books;
   for(int i=0;i<n;i++){
      ifile.getline(buf, 100);
      books.push_back(Book(buf));
   }

   sort(books.begin(), books.end());
   for (int i=0; i<n; i++)
      ofile << books[i] << endl;
}
