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
#include "myHash.h"

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr{

	public:
		CirMgr(){

			size_t size=5113;
			_hash=new Hash<HashKey, CirGateV>(size);

		}
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
		void printFECPairs() const;
		void writeAag(ostream&) const;
		void printSimValue(size_t gid);

	private:
		void processLine(vector<string>, size_t, string);
		CirGateV* searchInList(const int, const int) const; // type: 1 input, 2 output, 3 aig, 4 undef
		bool removeFromList(const int, const int, bool deleteIt=false) ;
			// type: 1 input, 2 output, 3 aig, 4 undef ; defined in cirOpt.cpp
		void deepFirstSearch(CirGateV*);
		void reset(){
		
			input.clear();
			output.clear();
			aig.clear();
			undef.clear();
			totalList.clear();
			delete [] _idList;		

		}
		void checkRedundent(CirGateV* x);
		void merge(CirGateV, CirGateV, bool oldInv=false);
		void updateTotalList();
		void simulation(unsigned int*, size_t, size_t);
		void createFecGroup(size_t);
		void continueGroup(size_t);
		void group(vector<CirGateV>*&, size_t, size_t);	
		size_t trimStr(string&);
		void writeLogFile(unsigned int* input, size_t inputSize,
   		unsigned int* output, size_t outputSize, size_t length);
		void printErrorMsg(string, size_t, size_t);
		void cleanFecGroups();
		void removeFromFecGroup(vector<CirGateV>*);
		void genProofModel(SatSolver&);

		ofstream *_simLog;
		size_t M, I, L, O, A;
		bool *flag;
		vector<CirGateV*> input;
		vector<CirGateV*> output;
		vector<CirGateV*> aig;
		vector<CirGateV*> undef;
		vector<CirGateV*> totalList;
		CirGateV** _idList;
		bool needToUpdate;
		string _fileName;
		Hash<HashKey, CirGateV>* _hash;
		unsigned int* simulationValue;
		vector< vector<CirGateV>* >* _fecGroup;
		size_t _simNum;

};

#endif // CIR_MGR_H
