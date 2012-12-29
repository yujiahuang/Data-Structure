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
#include "cirGate.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr{

	public:
		CirMgr(){}
		~CirMgr() {

			reset();

		}

		// Access functions
		// return '0' if "gid" corresponds to an undefined gate.
		CirGate* getGate(unsigned gid) const;

		// Member functions about circuit construction
		bool readCircuit(const string&);

		// Member functions about circuit optimization
		// defined in cirOpt.cpp
		void sweep();
		void optimize();

		// Member functions about simulation
		void randomSim();
		void fileSim(ifstream&);
		void setSimLog(ofstream *logFile) { _simLog = logFile; }

		// Member functions about fraig
		void strash();
		void printFEC() const;
		void fraig();

		// Member functions about circuit reporting
		void printSummary() const;
		void printNetlist() const;
		void printPIs() const;
		void printPOs() const;
		void printFloatGates() const;
		void printFECPairs() const{}
		void writeAag(ostream&) const;

	private:
		void processLine(vector<string>, size_t, string);
		CirGateV* searchInList(const int, const int) const; // type: 1 input, 2 output, 3 aig, 4 undef
		bool removeFromList(const int, const int) ;         // type: 1 input, 2 output, 3 aig, 4 undef ; defined in cirOpt.cpp
		void deepFirstSearch(CirGateV*);
		void reset(){
		
			input.clear();
			output.clear();
			aig.clear();
			undef.clear();
			totalList.clear();
		
		}
		void checkRedundent(CirGateV* x);

		ofstream *_simLog;
		size_t M, I, L, O, A;
		bool *flag;
		vector<CirGateV*> input;
		vector<CirGateV*> output;
		vector<CirGateV*> aig;
		vector<CirGateV*> undef;
		vector<CirGateV*> totalList;
		bool needToUpdate;
		string _fileName;

};

#endif // CIR_MGR_H
