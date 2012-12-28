/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2012 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/

// Remove unused gates
void CirMgr::sweep(){

	// flag those in totalList
	delete []flag;
	flag=new bool[M+1];
	for(size_t i=0; i<M+1; i++) flag[i]=0;

	for(vector<CirGateV*>::iterator it=totalList.begin(); it!=totalList.end(); it++){

		CirGate *gate = (*it)->gate();
		if(dynamic_cast<CirAigGate*>(gate)){
			
			flag[gate->getId()]=true;;
		
		}

	}

	// delete those not in totalList
	for(size_t i=1; i<M+1; i++){
	
		if(flag[i]==false){ // not in totalList
		
			if(removeFromList(3, 2*i)) cout << "Sweeping: AIG" << "(" << i << ") removed...\n";
			if(removeFromList(4, 2*i)) cout << "Sweeping: UNDEF" << "(" << i << ") removed...\n";
		
		}
	
	}

}

bool CirMgr::removeFromList(const int type, const int literal){

	vector<CirGateV *> *toBeSearched;
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

	for(vector<CirGateV *>::iterator it = toBeSearched->begin(); it!=toBeSearched->end(); it++){

		if((*it)->gate()->_id == (size_t)literal/2){
		
			toBeSearched->erase(it);
			return true;

		}

	}

	return false;

}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
void
CirMgr::optimize()
{
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

