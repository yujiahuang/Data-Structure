/****************************************************************************
	FileName     [ cirMgr.h ]
	PackageName  [ cir ]
	Synopsis     [ Define circuit manager ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2008-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr{

	public:
		CirMgr(){}
		~CirMgr() {}

		// Access functions
		// return '0' if "gid" corresponds to an undefined gate.
		CirGate* getGate(unsigned gid) const { return 0; }

		// Member functions about circuit construction
		bool readCircuit(const string&);

		// Member functions about circuit reporting
		void printSummary() const;
		void printNetlist() const;
		void printPIs() const;
		void printPOs() const;
		void printFloatGates() const;
		void writeAag(ostream&) const;

	private:
		void processLine(vector<string>, size_t);
		vector<CirGateV *>::iterator searchInList(const int, const int); // type: 1 input, 2 output, 3 aig 
		size_t M, I, L, O, A;
		vector<CirGateV*> input;
		vector<CirGateV*> output;
		vector<CirGateV*> aig;
		string _fileName;

};

#endif // CIR_MGR_H
