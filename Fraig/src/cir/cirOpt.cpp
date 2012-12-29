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
	if(flag!=0) delete []flag;
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
void CirMgr::optimize(){
	
	for(vector<CirGateV*>::iterator it=output.begin(); it!=output.end(); it++){
	
		checkRedundent((*it)->gate()->_faninList[0]);
		
	}

	// update totalList
	if(needToUpdate){

		totalList.clear();
		// dfs
		A=0;
		if(flag!=0) delete [] flag;
		flag=new bool[M+O+1];
		for(size_t i=0; i<M+O+1; i++) flag[i]=0;

		for(vector<CirGateV*>::iterator it=output.begin(); it!=output.end(); it++){

			deepFirstSearch((*it));

		}
		needToUpdate=false;

	}

}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

void CirMgr::checkRedundent(CirGateV* x){

	CirGate *gate = x->gate();
	//cout << "checking: " << gate->getId() << endl;
	// fanin list exists and no undef
	bool fanin = (gate->_faninList.size()==2) && !(gate->_faninList[0]->isUndef()) && !(gate->_faninList[1]->isUndef());
	
	if(dynamic_cast<CirAigGate*>(gate) && fanin){
	
		// find or create const 0
		CirGateV* const0 = searchInList(4, 0);
		if(const0==0 || const0->isInv()==true){
		
			CirAigGate *const0G = new CirAigGate( 0 );
			const0 = new CirGateV(const0G, 0);
			undef.push_back(const0);

		}

		// check condition
		CirGateV *result=0;
		bool sameId = gate->_faninList[0]->gate()->getId() == gate->_faninList[1]->gate()->getId(); 	
		bool inverse = gate->_faninList[0]->isInv() != gate->_faninList[1]->isInv(); 

		if(sameId && inverse) result = const0;
		else if (sameId && !inverse) result = gate->_faninList[0];
		else{

			for(size_t i=0; i<2; i++){

				if(gate->_faninList[i]->gate()->getId()==0) result = const0;
				else if(gate->_faninList[i]->gate()->getId()==0) result = (i==0) ? gate->_faninList[1] : gate->_faninList[0];

			}

		}
	
		// if need to change
		if(result!=0){
			
			needToUpdate=true;

			// erase fanout in result
			for(vector<CirGateV*>::iterator it=(result->gate()->_fanoutList).begin(); it!=(result->gate()->_fanoutList).end(); it++){

				if((*it)->gate()==gate){
				
					(result->gate()->_fanoutList).erase(it);	
					break;

				}

			}

			int whichOne;
			for(size_t i=0; i<gate->_fanoutList.size(); i++){
			
				// find out the one to add/replace
				CirGateV* fanout = gate->_fanoutList[i];
				if(fanout->gate()->_faninList[0]->gate()==gate) whichOne=0;
				else if(fanout->gate()->_faninList[1]->gate()==gate) whichOne=1;
				
				// replace
				fanout->gate()->_faninList[whichOne]=result; // replace fanin
				result->gate()->_fanoutList.push_back(fanout); // add fanout
				
			}

			// erase gate
			removeFromList(3, gate->getId()*2+x->isInv());

			// recursive
			if(result->gate()->getId()!=0) checkRedundent(result);

		}
		else{
		
			// recursive
			checkRedundent(gate->_faninList[0]);
			checkRedundent(gate->_faninList[1]);

		}

	}

}








