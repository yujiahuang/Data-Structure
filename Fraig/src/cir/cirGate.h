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
#include "sat.h"

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
		CirGate() {_flag=false;}
		CirGate(size_t i) : _id(i){_flag=false;};
		virtual ~CirGate() {}

		// Basic access methods
		string getTypeStr() const { return ""; }
		unsigned getLineNo() const { return 0; }

		// Printing functions
		virtual void printGate() const = 0;
		virtual void reportGate() const = 0;
		virtual void printSelfForReport() const = 0;
		void reportFanin(int level);
		void reportFanout(int level);
		void reportAndRecord(int, int, int);
		void resetFlag();

		//getter and setter
		size_t getId(){return _id;}
		size_t getLineNum(){return _lineNum;}
		void   setLineNum(size_t newLN){_lineNum=newLN;}
		const  string& getName(){return _name;}
		void   setName(const string& name){_name=name;}
		bool   getFlag(){return _flag;}
		const  vector<CirGateV*>& getFaninList() const { return _faninList;}

		// sat
		Var  getVar() const { return _var; } 
		void setVar(const Var& v) { _var = v; }  

	private:
		unsigned   _gid;
		Var        _var;    

	protected:
		size_t               _id;
		size_t              _lineNum;
		string              _name;
		bool                _flag;
		vector<CirGateV*>   _faninList;
		vector<CirGateV*>   _fanoutList;
		
};

// Pointer Wrapper
class CirGateV {

	public:
		CirGateV(CirGate* g, size_t phase, size_t undef=0):
			_gateV(size_t(g) + phase + 2*undef) { }

		CirGate* gate() const { 
	
			size_t i = (~0x0)-(0x3);
			return (CirGate*)(_gateV & i); 
			
		}
		size_t getGateV() const { return _gateV; }
		bool isInv() const { return (_gateV & 0x1); }
		bool isUndef() const { return (_gateV & 0x2); }
		void setUndef() { if(!isUndef()) _gateV+=2; }
		void setInv() { if(!isInv()) _gateV = _gateV | 0x1; }
		void resetInv() { if(isInv()) _gateV-=1; }
		void resetUndef() { if(isUndef()) _gateV-=2; }
	private:
		size_t _gateV;

};

// Derived Class
class CirAigGate : public CirGate {

	public:
		CirAigGate():CirGate(){}
		CirAigGate(size_t i) : CirGate(i){};
		~CirAigGate(){}
		void printGate() const;
		void reportGate() const;
		void printSelfForReport() const;
	
	private:

};

class CirPiGate : public CirGate {

	public:
		CirPiGate():CirGate(){}
		CirPiGate(size_t i) : CirGate(i){};
		~CirPiGate(){}
		void printGate() const;
		void reportGate() const;
		void printSelfForReport() const;

	private:

};

class CirPoGate : public CirGate {

	public:
		CirPoGate():CirGate(){}
		CirPoGate(size_t i) : CirGate(i){};
		~CirPoGate(){}
		void printGate() const;
		void reportGate() const;
		void printSelfForReport() const;

	private:

};





#endif // CIR_GATE_H
