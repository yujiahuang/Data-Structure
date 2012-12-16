/****************************************************************************
	FileName     [ cirGate.cpp ]
	PackageName  [ cir ]
	Synopsis     [ Define class CirAigGate member functions ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2008-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

/**************************************/
/*   class CirGate member functions   */
/**************************************/

void CirGate::reportFanin(int level) {

	assert (level >= 0);	
	printSelfForReport();
	cout << "\n  ";		
	
	for(vector<CirGateV*>::const_iterator it=_faninList.begin(); it!=_faninList.end(); it++){
	
		if((*it)->isUndef()) cout << "UNDEF " << (*it)->gate()->getId() << endl;
		else{

			if((*it)->isInv()) cout << "!";
			(*it)->gate()->reportAndRecord(level-1, 1, 1);
		
		}

	}
	cout << endl;
	resetFlag();

}

void CirGate::reportFanout(int level) {

	assert (level >= 0);	
	printSelfForReport();
	cout << "\n  ";		
	
	for(vector<CirGateV*>::const_iterator it=_fanoutList.begin(); it!=_fanoutList.end(); it++){
	
		if((*it)->isUndef()) cout << "UNDEF " << (*it)->gate()->getId() << endl;
		else{

			if((*it)->isInv()) cout << "!";
			(*it)->gate()->reportAndRecord(level-1, 1, 2);
		
		}

	}
	cout << endl;
	resetFlag();

}
void CirGate::reportAndRecord(int level, int recurseN, int type){ // 1 fanin, 2 fanout

	if(level >= 0){
	
		printSelfForReport();
		if(level >= 1){ 

			vector<CirGateV*> *v;
			if(type==1) v=&_faninList;
			else if(type==2) v=&_fanoutList;
			for(vector<CirGateV*>::iterator it=v->begin(); it!=v->end(); it++){ // print fanin
			
				cout << endl;
				cout << setw(2*(recurseN+1)) << " ";
				CirGate *gate = (*it)->gate();
				if((*it)->isUndef()){
				
					cout << "UNDEF " << gate->getId();
					cout.flush();

				}
				else if(gate->getFlag()==true){
				
					gate->printSelfForReport();
					cout << " (*)";
					cout.flush();

				}
				else{

					if((*it)->isInv()) cout << "!";
					gate->reportAndRecord(level-1, recurseN+1, type);
					cout.flush();
				
				}

			}	

			_flag=true;

		}

	}

}

void CirGate::resetFlag(){

	_flag=false;
	for(vector<CirGateV*>::iterator it=_faninList.begin(); it!=_faninList.end(); it++){
	
		(*it)->gate()->resetFlag();
	
	}

}

/********************/
/******* aig ********/
/********************/

void CirAigGate::printGate() const{

	cout << "AIG " << _id;
	for(vector<CirGateV*>::const_iterator it=_faninList.begin(); it!=_faninList.end(); it++){

		cout << ((*it)->isUndef() ? " *" : " ");
		cout << ((*it)->isInv() ? "!" : "");
		cout << (*it)->gate()->getId();

	}

}

void CirAigGate::reportGate() const{

	bool name=false;
	cout << "==================================================\n";

	if(_name.size()==0) name=false; 
	else name=true;

	int printed=0;

	if(!name) printed=printf("= AIG(%d), line %d", _id, _lineNum);
	else printed=printf("= AIG(%d)\"%s\", line %d", _id, _name.c_str(), _lineNum);
	for(int i=0; i<50-printed-1; i++) cout << " ";
	cout << "=\n==================================================\n";

}

void CirAigGate::printSelfForReport() const {

	if(_id==0) cout << "CONST0";
	else cout << "AIG " << _id;
	cout.flush();

}

/********************/
/******** pi ********/
/********************/

void CirPiGate::printGate() const{

	cout << "PI  " << _id;
	if(_name.size()!=0) cout << " (" << _name << ")";

}

void CirPiGate::reportGate() const{

	bool name=false;
	cout << "==================================================\n";

	if(_name.size()==0) name=false; 
	else name=true;

	int printed=0;

	if(!name) printed=printf("= PI(%d), line %d", _id, _lineNum);
	else printed=printf("= PI(%d)\"%s\", line %d", _id, _name.c_str(), _lineNum);
	for(int i=0; i<50-printed-1; i++) cout << " ";
	cout << "=\n==================================================\n";

}

void CirPiGate::printSelfForReport() const {

	cout << "PI " << _id;
	cout.flush();

}

/********************/
/******** po ********/
/********************/

void CirPoGate::printGate() const{

	cout << "PO  " << _id; 
	cout << (_faninList[0]->isInv() ? " !" : " ");
	cout << _faninList[0]->gate()->getId();
	if(_name.size()!=0) cout << " (" << _name << ")";

}

void CirPoGate::reportGate() const{

	bool name=false;
	cout << "==================================================\n";

	if(_name.size()==0) name=false; 
	else name=true;

	int printed=0;

	if(!name) printed=printf("= PO(%d), line %d", _id, _lineNum);
	else printed=printf("= PO(%d)\"%s\", line %d", _id, _name.c_str(), _lineNum);
	for(int i=0; i<50-printed-1; i++) cout << " ";
	cout << "=\n==================================================\n";

}

void CirPoGate::printSelfForReport() const{

	cout << "PO " << _id;
	cout.flush();

}
