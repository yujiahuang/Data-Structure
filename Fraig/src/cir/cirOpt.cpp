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
		
			if(removeFromList(3, 2*i, true)) cout << "Sweeping: AIG" << "(" << i << ") removed...\n";
			if(removeFromList(4, 2*i, true)) cout << "Sweeping: UNDEF" << "(" << i << ") removed...\n";	
	
		}
	
	}

}

bool CirMgr::removeFromList(const int type, const int literal, bool deleteIt){

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
		
			if(deleteIt) delete (*it)->gate();
			toBeSearched->erase(it);
			return true;

		}
	}

	return false;

}

// _dfsList needs to be reconstructed afterwards
void CirMgr::optimize(){

	if(_fecGroup!=0) return;

	for(vector<CirGateV*>::iterator it=totalList.begin(); it!=totalList.end(); it++){

		checkRedundent((*it));
	
	}

	// update totalList
	if(needToUpdate){
		updateTotalList();

	}

}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

void CirMgr::checkRedundent(CirGateV* x){

	CirGate *gate = x->gate();

	// fanin list exists and no undef
	bool fanin = (gate->_faninList.size()==2) 
							&& !(gate->_faninList[0]->isUndef()) 
							&& !(gate->_faninList[1]->isUndef());
	
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
		bool sameId = gate->_faninList[0]->gate()->getId() 
									== gate->_faninList[1]->gate()->getId(); 	
		bool inverse = gate->_faninList[0]->isInv() 
									!= gate->_faninList[1]->isInv(); 

		if(sameId && inverse) result = const0;
		else if (sameId && !inverse) result = gate->_faninList[0];
		else{
	
			for(size_t i=0; i<2; i++){
				
				CirGateV* g = gate->_faninList[i];
				if(g->gate()->getId()==0 && g->isInv()==0) result = const0;
				else if(g->gate()->getId()==0 && g->isInv()==1)
					result = ((i==0) ? gate->_faninList[1] : gate->_faninList[0]);

			}
		}
		
		// if need to change
		if(result!=0){
			
			needToUpdate=true;
			cout << "Simplifying: " << result->gate()->getId()
					 << " merging " << (result->isInv() ? "!" : "") <<  gate->getId() << "..." << endl;				
			merge(*x, *result);
		}
	}
}

void CirMgr::merge(CirGateV oldOne, CirGateV newOne){

	// remove
	removeFromList(3, 2*(oldOne.gate()->getId()));

	// erase fanout from fanins
	for(size_t i=0; i<2; i++){

		CirGateV* g = oldOne.gate()->_faninList[i];
		for(vector<CirGateV*>::iterator it=(g->gate()->_fanoutList).begin();
				it!=(g->gate()->_fanoutList).end(); it++){

			if((*it)->gate()==oldOne.gate()){

				(g->gate()->_fanoutList).erase(it);	
				break;

			}
		}
	}

	// merge fanouts
	size_t s = oldOne.gate()->_fanoutList.size();
	CirGateV newOneI=CirGateV(newOne.gate(), !newOne.isInv());
	for(size_t i=0; i < s; i++){

		CirGateV* oldFO=oldOne.gate()->_fanoutList[i];
		for(vector<CirGateV*>::iterator it=oldFO->gate()->_faninList.begin();
				it!=oldFO->gate()->_faninList.end(); it++){ // relink fanins

			if((*it)->gate()->getId()==oldOne.gate()->getId()){

				bool inv1 = (*it)->isInv();
				bool inv2 = oldOne.isInv();
				bool inv = inv1 ^ inv2;

				if(inv){
				
					*(*it)=newOneI;
					newOneI.gate()->_fanoutList.push_back(oldFO);

				}
				else{
				
				  *(*it)=newOne;
					newOne.gate()->_fanoutList.push_back(oldFO);

				}
			}
		}
	}	
}

void CirMgr::updateTotalList(){

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


