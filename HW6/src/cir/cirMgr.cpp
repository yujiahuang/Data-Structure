/****************************************************************************
	FileName     [ cirMgr.cpp ]
	PackageName  [ cir ]
	Synopsis     [ Define cir manager functions ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2008-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
	EXTRA_SPACE,
	MISSING_SPACE,
	ILLEGAL_WSPACE,
	ILLEGAL_NUM,
	ILLEGAL_IDENTIFIER,
	ILLEGAL_SYMBOL_TYPE,
	ILLEGAL_SYMBOL_NAME,
	MISSING_NUM,
	MISSING_IDENTIFIER,
	MISSING_NEWLINE,
	MISSING_DEF,
	CANNOT_INVERTED,
	MAX_LIT_ID,
	REDEF_GATE,
	REDEF_SYMBOLIC_NAME,
	REDEF_CONST,
	NUM_TOO_SMALL,
	NUM_TOO_BIG,

	DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

	static bool
parseError(CirParseError err)
{
	switch (err) {
		case EXTRA_SPACE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Extra space character is detected!!" << endl;
			break;
		case MISSING_SPACE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Missing space character!!" << endl;
			break;
		case ILLEGAL_WSPACE: // for non-space white space character
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Illegal white space char(" << errInt
				<< ") is detected!!" << endl;
			break;
		case ILLEGAL_NUM:
			cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
				<< errMsg << "!!" << endl;
			break;
		case ILLEGAL_IDENTIFIER:
			cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
				<< errMsg << "\"!!" << endl;
			break;
		case ILLEGAL_SYMBOL_TYPE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Illegal symbol type (" << errMsg << ")!!" << endl;
			break;
		case ILLEGAL_SYMBOL_NAME:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Symbolic name contains un-printable char(" << errInt
				<< ")!!" << endl;
			break;
		case MISSING_NUM:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Missing " << errMsg << "!!" << endl;
			break;
		case MISSING_IDENTIFIER:
			cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
				<< errMsg << "\"!!" << endl;
			break;
		case MISSING_NEWLINE:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": A new line is expected here!!" << endl;
			break;
		case MISSING_DEF:
			cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
				<< " definition!!" << endl;
			break;
		case CANNOT_INVERTED:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": " << errMsg << " " << errInt << "(" << errInt/2
				<< ") cannot be inverted!!" << endl;
			break;
		case MAX_LIT_ID:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
				<< endl;
			break;
		case REDEF_GATE:
			cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
				<< "\" is redefined, previously defined as "
				<< errGate->getTypeStr() << " in line " << errGate->getLineNo()
				<< "!!" << endl;
			break;
		case REDEF_SYMBOLIC_NAME:
			cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
				<< errMsg << errInt << "\" is redefined!!" << endl;
			break;
		case REDEF_CONST:
			cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
				<< ": Cannot redefine const (" << errInt << ")!!" << endl;
			break;
		case NUM_TOO_SMALL:
			cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
				<< " is too small (" << errInt << ")!!" << endl;
			break;
		case NUM_TOO_BIG:
			cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
				<< " is too big (" << errInt << ")!!" << endl;
			break;
		default: break;
	}
	return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool CirMgr::readCircuit(const string& fileName){

	_fileName=fileName;
	ifstream circuitF(fileName.c_str());
	string line;
	size_t lineNum=1;
	while(getline(circuitF, line)){

		vector<string> tokens;
		char *tok = strtok (line.c_str()," ");
		while (pch != NULL){

			tokens.push_back(string(tok));
			tok = strtok (NULL, " ,.-");
		
		}
		processLine(tokens, lineNum);
		lineNum++;

	}

	return true;

}

void processLine(vector<string> tokens, size_t lineNum){

	if(lineNum==1 && tokens[0].compare("aag")==0){
	
		if(tokens.size()==6){
		
			M=tokens[1];
			I=tokens[2];
			L=tokens[3];
			O=tokens[4];
			A=tokens[5];
		
		}
		else; // error	

	}	
	else if(lineNum>1 && lineNum<=1+I){ // input
	
		if(tokens.size()==1){
		
			int literal = atoi(tokens[0].c_str);
			CirPiGate *pi = new CirPiGate(literal/2);
			CirGateV *gatePtr = new CirGateV(pi, literal%2);
			input.push_back(gatePtr);

		}
		else; // error
	
	}
	else if(lineNum>1+I && lineNum<=1+I+L+O){ // output
	
		if(tokens.size()==1){
		
			int literal = atoi(tokens[0].c_str);
			CirAigGate *aig = new CirAigGate(literal/2);
			CirGateV *aigGatePtr = new CirGateV(aig, literal%2);
			aig.push_back(aigGatePtr);

			CirPoGate *po = new CirPoGate( M+(lineNum-I-1) );
			po->_fanin=*(aig.end()-1);

			CirGateV *poGatePtr = new CirGateV(po, 0);
			output.push_back(poGatePtr);

		}
		else; //error
	
	}
	else if(lineNum>1+I+L+O && lineNum<=1+I+L+O+A){ // aig
	
		if(tokens.size()==3){
		
			int lhs = atoi(tokens[0].c_str);
			int rhs1 = atoi(tokens[1].c_str);
			int rhs2 = atoi(tokens[2].c_str);

			vector<CirGateV*>::iterator it = searchInList(3, literal);
			if(it==0){

				CirAigGate *aig = new CirAigGate(literal/2);
				CirGateV *aigGatePtr = new CirGateV(aig, literal%2);

				aig.push_back(aigGatePtr);

				//TODO connect aig

			}else{
			
				
			
			}

		
		}
		else; //error
	
	}

}

vector<CirGateV *>::iterator searchInList(const int type, const int literal){

	vector<CirGateV *> *toBeSearched;
	if(type==1){ // input

		toBeSearched=&input;

	}
	else if(type==2){ // output
	
		toBeSearched=&output;
	
	}
	else if(type==3){ // aig
	
		toBeSearched=&aigput;
	
	}
	else; //error

	for(vector<CirGateV *>::iterator it = toBeSearched->begin(); it!=toBeSearched->end(); it++){

		if((*it)->gate()->_id*2+(*it)->isInv() == literal) return it;

	}

	return 0;

}



/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
	Circuit Statistics
	==================
	PI          20
	PO          12
	AIG        130
	------------------
	Total      167
 *********************/
void
CirMgr::printSummary() const
{
}

void
CirMgr::printNetlist() const
{
}

void
CirMgr::printPIs() const
{
	cout << "PIs of the circuit:";
	cout << endl;
}

void
CirMgr::printPOs() const
{
	cout << "POs of the circuit:";
	cout << endl;
}

void
CirMgr::printFloatGates() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
}
