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
static string mp="12345678901234567890123456789012345678901234567890123456789012345FCDEVGHIPKLMNOBQRSTUJWXAYZ"; // TODO

// xMap(Original) ==> Mapped alphabet
// Need to convert everything Upper-case
// return original character if it is not an alphabet
static char xMap(char c) // TODO
{
   if(97 <= c && c <= 122){
	 
	 		c-=32;
			c=mp[c];
	 
	 }else if(65 <= c && c <= 90){
	 
	 		c=mp[c];
	 
	 }
	 
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
   
			for(int i=0; i<size(); i++){
			
				_str[i]=xMap(_str[i]);
			
			}   		
	 
	 }// TODO

   size_t size() const { 
	 
		 return _str.length(); 
	 
	 } // TODO
   bool operator < (const xStr& s) const { 
	 
	   bool smaller;
		 int cmp=_str.compare(s._str);
	   if(cmp>0) smaller=false;
		 else if(cmp<0) smaller=true;
	 	 return smaller;

	 } // TODO
   bool operator == (const xStr& s) const { 
	 
		 bool diff=_str.compare(s._str);

	 	 return !diff; 
	 
	 } // TODO

   friend ostream& operator << (ostream& os, const xStr& s);

private:
   string _str; // converted string
};

ostream& operator << (ostream& os, const xStr& s)
{
   // TODO
	 os << s._str;

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
	 int comma=0, parenthesis=0;
	 for(; s[comma]!=','; comma++);
	 for(; s[parenthesis]!='('; parenthesis++);
	 _author=xStr(s.substr(0, comma));
	 _book=xStr(s.substr(comma+2, parenthesis-comma-3));
	 _year=atoi(s.substr(parenthesis+1, 4).c_str());

}

bool Book::operator < (const Book& b) const
{
   // TODO
	 bool smaller;
	 if(!(_author == b._author)){
	 
	   smaller=(_author < b._author);
	 
	 }
	 else if(_year != b._year){
	 
	   smaller=(_year < b._year);
	 
	 }
	 else if(!(_book == b._book)){
	 
	   smaller=(_book < b._book);
	 
	 }

	 return smaller;

}

ostream& operator << (ostream& os, const Book& b)
{
   // TODO
	 os << b._str;

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
