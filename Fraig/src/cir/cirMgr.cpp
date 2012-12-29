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

CirGate* CirMgr::getGate(unsigned gid) const{

	CirGate *gate=0;
	for(size_t i=1; i<=3 && gate==0; i++){

		CirGateV *gateV=searchInList(i, 2*gid);
		if(gateV!=0) gate = gateV->gate();	

	}

	return gate;

}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool CirMgr::readCircuit(const string& fileName){

	_fileName=fileName;
	ifstream circuitF(fileName.c_str());
	
	if(circuitF.fail()) return false;
	
	string line;
	size_t lineNum=1;
	while(getline(circuitF, line)){

		char *lineArr = new char[line.size()];
		strcpy(lineArr, line.c_str());
		vector<string> tokens;
		char *tok = strtok (lineArr," ");
		while (tok != NULL){

			tokens.push_back(string(tok));
			tok = strtok (NULL, " ,.-");
		
		}
		if(tokens[0].compare("c")!=0) // not comment
			processLine(tokens, lineNum, line);
		else break;
		lineNum++;

	}
	
	// set undef
	for(vector<CirGateV*>::iterator it=undef.begin(); it!=undef.end(); it++){

		if((*it)->gate()->getId()!=0) (*it)->setUndef();

	}

	// dfs
	A=0;
	if(flag!=0) delete []flag;
	flag=new bool[M+O+1];

	for(size_t i=0; i<M+O+1; i++) flag[i]=0;
	
	for(vector<CirGateV*>::iterator it=output.begin(); it!=output.end(); it++){

		deepFirstSearch((*it));
		
	}
	needToUpdate=false;

	// close file
	circuitF.clear();
	circuitF.close();

	return true;

}

void CirMgr::processLine(vector<string> tokens, size_t lineNum, string origin){

	if(lineNum==1 && tokens[0].compare("aag")==0){ // header
	
		if(tokens.size()==6){
		
			M=atoi(tokens[1].c_str());
			I=atoi(tokens[2].c_str());
			L=atoi(tokens[3].c_str());
			O=atoi(tokens[4].c_str());
			A=atoi(tokens[5].c_str());
		
		}
		else; // error	

	}	
	else if(lineNum>1 && lineNum<=1+I){ // input
	
		if(tokens.size()==1){
		
			int literal = atoi(tokens[0].c_str());
			CirPiGate *pi;
			CirGateV *gatePtr;
			if(searchInList(1, literal)==0){
			
				pi = new CirPiGate(literal/2);
				pi->setLineNum(lineNum);
				gatePtr = new CirGateV(pi, literal%2);
				input.push_back(gatePtr);

			}

		}
		else; // error
	
	}
	else if(lineNum>1+I && lineNum<=1+I+L+O){ // output
	
		if(tokens.size()==1){
		
			int literal = atoi(tokens[0].c_str());
			CirAigGate *aigGate;
			CirGateV *aigGatePtr;
			CirGateV* searchResult = searchInList(3, literal);
			if(searchResult==0) searchResult = searchInList(4, literal);

			if(searchResult==0){
			
				aigGate = new CirAigGate(literal/2);
				aigGatePtr = new CirGateV(aigGate, literal%2);
				undef.push_back(aigGatePtr);
	
			}
			else if(searchResult->isInv() != literal%2){
			
				aigGatePtr = new CirGateV(searchResult->gate(), literal%2);
			
			}
			else{
			
				aigGatePtr = searchResult;
			
			}

			CirPoGate *po = new CirPoGate( M+(lineNum-I-1) );
			po->setLineNum(lineNum);
			po->_faninList.push_back(aigGatePtr);

			CirGateV *poGatePtr = new CirGateV(po, 0);
			output.push_back(poGatePtr);
			aigGate->_fanoutList.push_back(poGatePtr);
		
		}
		else; //error
	
	}
	else if(lineNum>1+I+L+O && lineNum<=1+I+L+O+A){ // aig
	
		if(tokens.size()==3){
		
			// declaration
			int lhs = atoi(tokens[0].c_str());
			int rhs[2];
			rhs[0] = atoi(tokens[1].c_str());
			rhs[1] = atoi(tokens[2].c_str());

			// check if lhs already exists
			CirGateV* gateL = searchInList(3, lhs);
			CirAigGate *aigGate;
			CirGateV *aigGatePtr;

			if(gateL==0){

				gateL=searchInList(4, lhs);
				if(gateL==0) aigGate = new CirAigGate(lhs/2);
				else{
				
					aigGate = (CirAigGate*)gateL->gate();
					for(vector<CirGateV *>::iterator it = undef.begin(); it!=undef.end(); it++){ // erase from undef
						
						if((*it)->gate()->_id == (size_t)lhs/2){
							undef.erase(it);
							break;
						}

					}

				}

			}
			else {
			
				aigGate = (CirAigGate*)gateL->gate();
			
			}

			aigGatePtr = new CirGateV(aigGate, 0);

			// check if rhs already exists
			CirGateV* gateR[2];
			gateR[0] = searchInList(1, rhs[0]); // in input
			gateR[1] = searchInList(1, rhs[1]);
			if(gateR[0]==0) gateR[0] = searchInList(3, rhs[0]); // in aig
			if(gateR[1]==0) gateR[1] = searchInList(3, rhs[1]);
			if(gateR[0]==0) gateR[0] = searchInList(4, rhs[0]); // in undef
			if(gateR[1]==0) gateR[1] = searchInList(4, rhs[1]);

			for(int i=0; i<=1; i++){

				if(gateR[i]==0){ // not found then create one and push into undef

					CirAigGate *aigR = new CirAigGate(rhs[i]/2);
					CirGateV *aigGatePtrR = new CirGateV(aigR, rhs[i]%2);
					aigR->_fanoutList.push_back(aigGatePtr);
					aigGate->_faninList.push_back(aigGatePtrR);
					undef.push_back(aigGatePtrR);

				}
				else if(gateR[i]->isInv() != rhs[i]%2) { // exist but inverted

					CirGateV *aigGatePtrR = new CirGateV(gateR[i]->gate(), rhs[i]%2);
					aigGate->_faninList.push_back(aigGatePtrR);
					gateR[i]->gate()->_fanoutList.push_back(aigGatePtr);

				}
				else{ // in aig then connect

					gateR[i]->gate()->_fanoutList.push_back(aigGatePtr);
					aigGate->_faninList.push_back(gateR[i]);

				}
		
			}
			
			aigGate->setLineNum(lineNum);
			aig.push_back(aigGatePtr);
		
		}
		else; //error

	
	}
	else{ // symbol and comment

		if(tokens.size()>=2){

			string type=tokens[0].substr(0, 1);
			string lineNumStr=tokens[0].substr(1, string::npos);
			size_t fix = 0;
			vector<CirGateV*> *v=0;
			if(type.compare("i")==0){ // input

				v=&input;
				fix = 2;

			}	
			else if(type.compare("o")==0){ // output

				v=&output;
				fix = I + 2;

			}
			else; //error

			if(v!=0){

				size_t n=atoi(lineNumStr.c_str())+fix;
				for(vector<CirGateV*>::iterator it=v->begin(); it!=v->end(); it++){

					CirGate *gate=(*it)->gate();
					if( gate->getLineNum()==n && gate->getName().size()==0 )
						gate->setName( origin.substr(    strchr(origin.c_str(),' ')-origin.c_str()+1, string::npos  )   );
					else; //error, either no line or set twice

				}

			}

		}
		else; // error

	}

}

CirGateV* CirMgr::searchInList(const int type, const int literal) const{

	const vector<CirGateV *> *toBeSearched;
	if(type==1){ // input

		toBeSearched=&input;

	}
	else if(type==2){ // output
	
		toBeSearched=&output;
	
	}
	else if(type==3){ // aig
	
		toBeSearched=&aig;
	
	}
	else if(type==4){ //undef

		toBeSearched=&undef;

	}
	else; //error

	for(vector<CirGateV *>::const_iterator it = toBeSearched->begin(); it!=toBeSearched->end(); it++){

		if((*it)->gate()->_id == (size_t)literal/2) return *it;

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
void CirMgr::printSummary() const{

	size_t i=input.size(), o=output.size(), a=aig.size();
	cout << "Circuit Statistics\n==================\nPI\t" << i 
	<< "\nPO\t" << o
	<< "\nAIG\t" << a 
	<< "\n------------------\nTotal\t" << i+o+a << endl;

}

void CirMgr::printNetlist() const{

	int i=0;
	for(vector<CirGateV*>::const_iterator it=totalList.begin(); it!=totalList.end(); it++){

		cout << "[" << i << "]" << " ";
	
		if((*it)->gate()->_id==0) cout << ( (*it)->isInv() ? "CONST0" : "CONST1" );
		else (*it)->gate()->printGate();

		cout << endl;
		i++;

	}	

}

void CirMgr::printPIs() const{

	cout << "PIs of the circuit:";
	for(vector<CirGateV*>::const_iterator it=input.begin(); it!=input.end(); it++) cout << " " << ((*it)->gate()->getId());
	cout << endl;

}

void CirMgr::printPOs() const{ 

	cout << "POs of the circuit:";
	for(vector<CirGateV*>::const_iterator it=output.begin(); it!=output.end(); it++) cout << " " << ((*it)->gate()->getId());
	cout << endl;
}

void CirMgr::printFloatGates() const{

	vector<int> v1;
	vector<int> v2;
	
	for(vector<CirGateV*>::const_iterator it = output.begin(); it!=output.end(); it++){
	
		CirGate *gate = (*it)->gate();
		bool missingFanin = gate->_faninList.size()<1;
		bool faninUndef = gate->_faninList[0]->isUndef();
		if(missingFanin || faninUndef) v1.push_back(gate->getId());
	
	}		
	for(vector<CirGateV*>::const_iterator it = aig.begin(); it!=aig.end(); it++){
	
		CirGate *gate = (*it)->gate();
		bool missingFanin = gate->_faninList.size()<2;
		bool faninUndef = gate->_faninList[0]->isUndef() || gate->_faninList[1]->isUndef();
		if(missingFanin || faninUndef) v1.push_back(gate->getId());
	
	}	

	if(v1.size()!=0){

		cout << "Gates with floating fanin(s):";
		for(vector<int>::iterator it=v1.begin(); it!=v1.end(); it++) cout << " " << (*it);

	}

	for(vector<CirGateV*>::const_iterator it = input.begin(); it!=input.end(); it++){
		
		CirGate *gate = (*it)->gate();
		bool missingFanout = gate->_fanoutList.size()==0;
		if(missingFanout) v2.push_back(gate->getId());

	}
	for(vector<CirGateV*>::const_iterator it = aig.begin(); it!=aig.end(); it++){
	
		CirGate *gate = (*it)->gate();
		bool missingFanout = gate->_fanoutList.size()==0;
		if(missingFanout) v2.push_back(gate->getId());
	
	}	

	if(v2.size()!=0){

		cout << "\nGates defined but not used :";
		for(vector<int>::iterator it=v2.begin(); it!=v2.end(); it++) cout << " " << (*it);

	}

}

void CirMgr::writeAag(ostream& outfile) const{

	// header
	outfile << "aag "
					<< M << " "
				  << I << " "
					<< L << " "
					<< O << " "
					<< A << endl;
	
	// PI
	for(vector<CirGateV*>::const_iterator it=input.begin(); it!=input.end(); it++){
	
		outfile << (*it)->gate()->getId()*2 << endl;
	
	}	

	// PO
	for(vector<CirGateV*>::const_iterator it=output.begin(); it!=output.end(); it++){
	
		CirGateV* gateV=(*it)->gate()->_faninList[0];
		outfile << gateV->gate()->getId()*2 + gateV->isInv() << endl;
	
	}

	// AIG
	for(vector<CirGateV*>::const_iterator it=totalList.begin(); it!=totalList.end(); it++){
	
		CirGate *gate=(*it)->gate();
		if(dynamic_cast<CirAigGate*>(gate) && gate->getId()!=0){
			
			outfile << gate->getId()*2;
			for(vector<CirGateV*>::const_iterator itf=gate->_faninList.begin(); itf!=gate->_faninList.end(); itf++){
			
				outfile << " " << (*itf)->gate()->getId()*2 + (*itf)->isInv();
			
			}
			outfile << endl;

		}

	}

	// symbol
	int i=0;
	for(vector<CirGateV*>::const_iterator it=input.begin(); it!=input.end(); it++){
	
		CirGate* gate=(*it)->gate();
		if(gate->getName().size()!=0){
			
			outfile << "i" << i << " " << gate->getName() << endl;
			i++;

		}

	}	
	i=0;
	for(vector<CirGateV*>::const_iterator it=output.begin(); it!=output.end(); it++){
		
		CirGate* gate=(*it)->gate();
		if(gate->getName().size()!=0){
			
			outfile << "o" << i << " " << gate->getName() << endl;
			i++;

		}
	
	}

}

void CirMgr::deepFirstSearch(CirGateV* x){

	if (flag[x->gate()->_id]==false){

		vector<CirGateV*> *v=&(x->gate()->_faninList);
		if(v->size()!=0){

			for(vector<CirGateV*>::iterator it=v->begin(); it!=v->end(); it++){

				if(!((*it)->isUndef())) deepFirstSearch(*it);
				else flag[(*it)->gate()->getId()]=true;

			}

		}

		flag[x->gate()->_id]=true;
		if(dynamic_cast<CirAigGate*>((x->gate()))) ++A;
		totalList.push_back(x);

	}

}

