/****************************************************************************
	FileName     [ calcModNum.h ]
	PackageName  [ calc ]
	Synopsis     [ Define class ModNum ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#ifndef CALC_MOD_NUM_H
#define CALC_MOD_NUM_H

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class ModNum;

typedef map<string, ModNum> CalcMap;

class ModNum
{
	public:
		// Default constructor
		// ==> Make sure _num is within [0, _modulus)
		// TODO: Implement the default constructor;
		ModNum(int i = 0){ if(i >= 0 && i<=_modulus) _num=i; else _num=0; }
		// Get the ModNum from the _varMap based on the string "str".
		// If found, copy the value.
		// If not found, create a default one and insert to the _varMap.
		ModNum(const string& str) { _num = _varMap[str]._num; }

		// Operator overload
		// TODO: overload the following operators (+, +=, -, -=, *, *=, ==, !=, =)
		//
		ModNum operator + (const ModNum& n) const { return ModNum(_num+n._num); }
		ModNum& operator += (const ModNum& n) { _num+=n._num; return (*this); }
		ModNum operator - (const ModNum& n) const { return ModNum(_num-n._num); }
		ModNum& operator -= (const ModNum& n) { _num-=n._num; return (*this); }
		ModNum operator * (const ModNum& n) const { return ModNum(_num*n._num); }
		ModNum& operator *= (const ModNum& n) { _num*=n._num; return (*this); }
		bool operator == (const ModNum& n) const { return (_num==n._num); }
		bool operator != (const ModNum& n) const { return (_num!=n._num); }
		ModNum& operator = (const ModNum& n) { _num=n._num; return (*this); }

		// static methods
		static void setModulus(int m) { _modulus = m; }
		static int getModulus() { return _modulus; }
		//
		// [TODO] Set the variable 's' in the _varMap to value 'n',
		// no matter the variable 's' exists in _varMap or not
		static void setVarVal(const string& s, const ModNum& n) {

			_varMap[s]=n;

		}
		//
		// [TODO] Get the value of variable 's'.
		// If 's' can be found, store the value in 'n' and return true.
		// Otherwise ('s' not found), return false.
		static bool getVarVal(const string& s, ModNum& n) {

			bool returnedValue=false;

			if(_varMap.find(s)!=_varMap.end()){

				returnedValue=true;
				n=_varMap[s];

			}
			else returnedValue=false;

			return returnedValue;
	 	
		}
		//
		// [TODO] If 's' is a valid variable name, return "getVarVal(s, n)";
		// else if 's' is a number, convert it to ModNum (e.g. by "myStr2Int") and
		// assign to 'n'
		static bool getStrVal(const string& s, ModNum& n) { 

			bool returnedValue=false;

			if(isValidVarName(s)) returnedValue=getVarVal(s, n); 
			else if(myStr2Int(s, n._num)) returnedValue=true;
			else returnedValue=false;

			return returnedValue; 

		}
		//
		// [TODO] Print out all the variables in _varMap, one variable per line,
		// in the following format ---
		// a = 9
		// b = 10
		// kkk = 18
		static void printVars() {

			for(CalcMap::iterator it=_varMap.begin(); it != _varMap.end(); it++){

				cout << (*it).first << " = " << (*it).second << endl;

			}	

		}
		static void resetVapMap() {}

		// friend functions
		friend ostream& operator << (ostream&, const ModNum&);  // TODO

	private:
		// Data members
		// DO NOT add/delete/modify data members
		int                _num;

		static int         _modulus;
		static CalcMap     _varMap;
};

#endif // CALC_MOD_NUM_H
