/****************************************************************************
	FileName     [ calcModNum.cpp ]
	PackageName  [ calc ]
	Synopsis     [ Define member functions for class ModNum ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include <iostream>
#include "util.h"
#include "calcModNum.h"

// TODO: Initialize the static data members of class ModNum
//       (Note: let default _modulus = 100000000)
// TODO: Define the member functions of class ModNum

int ModNum::_modulus = 100000000;
CalcMap ModNum::_varMap;





ostream& operator << (ostream& os, const ModNum& n){

	os << n._num;
	return os;

}


