/****************************************************************************
	FileName     [ cirFraig.cpp ]
	PackageName  [ cir ]
	Synopsis     [ Define cir FRAIG functions ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2010-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHash.h"
#include "util.h"

using namespace std;

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
void CirMgr::strash(){

	// create hash and do merging at the same time
	for(vector<CirGateV*>::iterator it=totalList.begin(); it!=totalList.end(); it++){
	
		// define type
		GateType t;
		if(dynamic_cast<CirPiGate*>((*it)->gate())) t=PI_GATE;
		else if(dynamic_cast<CirPoGate*>((*it)->gate())) t=PO_GATE;
		else if(dynamic_cast<CirAigGate*>((*it)->gate())){
		
			if((*it)->isUndef()) t=UNDEF_GATE;
			else t=AIG_GATE;

		}

		if(t==AIG_GATE){

			// merge / insert
			CirGateV gateV=*(*it);
			HashKey* key = new HashKey((*it)->gate(), t);
			if(_hash->check(*key, gateV)){ // if exists

				needToUpdate=true;
				CirGate* gate = (*it)->gate();
				size_t s = gateV.gate()->_fanoutList.size();
				for(size_t i=0; i < s; i++){ // merge output (move from gateV to (*it))

					CirGateV* newFO=gateV.gate()->_fanoutList[i];
					gate->_fanoutList.push_back(newFO);
					for(vector<CirGateV*>::iterator it2=newFO->gate()->_faninList.begin();
							it2!=newFO->gate()->_faninList.end(); it2++){ // relink fanins

						if((*it2)->gate()->getId()==gateV.gate()->getId()){
						
							*(*it2)=*(*it);
			
						}

					}

				}

				removeFromList(3, 2*gateV.gate()->getId() + gateV.isInv());
				delete gateV.gate();

			}
			else{

				_hash->insert(*key, gateV);

			}

		}
		
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

	void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
