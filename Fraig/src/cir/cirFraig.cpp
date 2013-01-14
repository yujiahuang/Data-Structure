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

				// print	
				cout << "Strashing: " << gateV.gate()->getId()
						 << " merging " << (*it)->gate()->getId() 
						 << "..." << endl;

				needToUpdate=true;
				merge(*(*it), gateV);

			}
			else{

				_hash->insert(*key, gateV);

			}
		}
	}

	// update totalList
	if(needToUpdate){

		updateTotalList();

	}

}

	void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
