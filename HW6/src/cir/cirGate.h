/****************************************************************************
	FileName     [ cirGate.h ]
	PackageName  [ cir ]
	Synopsis     [ Define basic gate data structures ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2008-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------

// Base Class
class CirGate{

	public:
		CirGate() {}
		CirGate(size_t i) : _id(i);
		virtual ~CirGate() {}

		// Basic access methods
		string getTypeStr() const { return ""; }
		unsigned getLineNo() const { return 0; }

		// Printing functions
		virtual void printGate() const = 0;
		void reportGate() const;
		void reportFanin(int level) const;
		void reportFanout(int level) const;

	private:

	protected:
		size_t _id;

};

// Pointer Wrapper
class CirGateV {

	public:
		CirGateV(CirGate* g, size_t phase):
			_gateV(size_t(g) + phase) { }

		CirGate* gate() const { return (CirGate*)(_gateV & 0xFFFFFFFC); }

		bool isInv() const { return (_gateV & 0x1); }

	private:
		size_t _gateV;

};

// Derived Class
class CirAigGate : CirGate {

	public:
		CirAigGate(){}
		~CirAigGate(){}

	private:
		vector<CirGateV> _faninList;
		vector<CirGateV> _fanoutList;

};

class CirPiGate {

	public:
		CirPiGate(){}
		~CirPiGate(){}

	private:
		vector<CirGateV> _fanoutList;
		char *_name;

};

class CirPoGate {

	public:
		CirPoGate(){}

	private:
		vector<CirGateV> _faninList;
		char *_name;

};





#endif // CIR_GATE_H
