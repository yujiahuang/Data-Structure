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
	for(vector<CirGateV*>::iterator it=_totalList.begin(); it!=_totalList.end(); it++){
	
		GateType t;
		if(dynamic_cast<CirPiGate*>((*it)->gate())) t=PI_GATE;
		else if(dynamic_cast<CirPoGate*>((*it)->gate())) t=PO_GATE;
		else if(dynamic_cast<CirAigGate*>((*it)->gate())){
		
			if((*it)->isUndef()) t=UNDEF_GATE;
			else t=AIG_GATE;

		}

		HashKey* key = new HashKey((it*)->gate(), t);
		_hash.insert(key, (*it));
	
	}

	// merge


}

	void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
