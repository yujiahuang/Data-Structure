#include<iostream>
#include"hw1.2.p1a.h"
using namespace std;

void P1a::assign(const string& s){

	_str=s;

}

void P1a::print() const{

	cout << endl << _str;

}

P1a& P1a::append(const P1a& p){

	(*this)._str.append(p._str);
	return *this;

} 
