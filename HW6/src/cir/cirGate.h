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
class CirGateV;

class CirGate{

friend class CirMgr;

	public:
		CirGate() {}
		CirGate(size_t i) : _id(i){};
		virtual ~CirGate() {}

		// Basic access methods
		string getTypeStr() const { return ""; }
		unsigned getLineNo() const { return 0; }

		// Printing functions
		virtual void printGate() const = 0;
		void reportGate() const;
		void reportFanin(int level) const;
		void reportFanout(int level) const;

		size_t getId(){return _id;}

	private:

	protected:
		size_t _id;
		vector<CirGateV*> _faninList;
		vector<CirGateV*> _fanoutList;


};

// Pointer Wrapper
class CirGateV {

	public:
		CirGateV(CirGate* g, size_t phase):
			_gateV(size_t(g) + phase) { }

		CirGate* gate() const { 
	
			size_t i = (~0x0)-(0x3);
			return (CirGate*)(_gateV & i); 
			
		}

		bool isInv() const { return (_gateV & 0x1); }

	private:
		size_t _gateV;

};

// Derived Class
class CirAigGate : public CirGate {

	public:
		CirAigGate(){}
		CirAigGate(size_t i) : CirGate(i){};
		~CirAigGate(){}
		void printGate() const;
	
	private:

};

class CirPiGate : public CirGate {

	public:
		CirPiGate(){}
		CirPiGate(size_t i) : CirGate(i){};
		~CirPiGate(){}
		void printGate() const;

	private:
		char *_name;

};

class CirPoGate : public CirGate {

	public:
		CirPoGate(){}
		CirPoGate(size_t i) : CirGate(i){};
		~CirPoGate(){}
		void printGate() const;

	private:
		char *_name;

};





#endif // CIR_GATE_H
