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

	// create hash
	for(vector<CirGateV*>::iterator it=totalList.begin(); it!=totalList.end(); it++){
	
		// define type
		GateType t;
		if(dynamic_cast<CirPiGate*>((*it)->gate())) t=PI_GATE;
		else if(dynamic_cast<CirPoGate*>((*it)->gate())) t=PO_GATE;
		else if(dynamic_cast<CirAigGate*>((*it)->gate())){
		
			if((*it)->isUndef()) t=UNDEF_GATE;
			else t=AIG_GATE;

		}

		// merge / insert
		CirGateV gateV=*(*it);
		HashKey* key = new HashKey((*it)->gate(), t);
		if(_hash->check(*key, gateV)){ // if exists

			CirGate* gate = (*it)->gate();
			for(size_t i=0; i<gateV.gate()->_fanoutList.size(); i++){ // merge output (move from gateV to (*it))

				CirGateV* newFO=gateV.gate()->_fanoutList[i];
				gate->_fanoutList.push_back(newFO);
				for(vector<CirGateV*>::iterator it2=newFO->gate()->_faninList.begin();
						it2!=newFO->gate()->_faninList.end(); it2++){ // relink fanins

					if((*it2)->getGateV()==gateV.getGateV()) (*it2)=(*it);

				}
							
			}			
			
		}
		else{

			_hash->insert(*key, gateV);

		}
	
	}

}

	void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
