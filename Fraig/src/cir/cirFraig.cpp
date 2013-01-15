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

void CirMgr::fraig(){

	if(_fecGroup==0) return;

	SatSolver solver;
	solver.initialize();

	genProofModel(solver);

	// Solve all FEC pairs
	bool result;

	bool *tmpFlag=new bool[M+1];
	for(size_t k=0; k<M+1; k++) tmpFlag[k]=0;

	for(vector< vector<CirGateV>* >::iterator it=_fecGroup->begin();
			it!=_fecGroup->end(); it++){

		if(tmpFlag[(*((*it)->begin())).gate()->getId()]==false){

			vector<CirGateV>* v = (*it);
			for(size_t i=0; i<(*it)->size()-1; i++){
			
				if((*v)[i].isUndef()) continue;
				for(size_t j=i+1; j<(*it)->size(); j++){

					if((*v)[j].isUndef()) continue;
	
					if((*v)[j].gate()->getId()==0){

						(*v)[j].gate()->setVar(0);

					}

					CirAigGate xorGate;
					Var var = solver.newVar();
					xorGate.setVar(var);

					solver.addXorCNF(xorGate.getVar(), (*v)[i].gate()->getVar(), (*v)[i].isInv(),
							(*v)[j].gate()->getVar(), (*v)[j].isInv());

					solver.assumeRelease();  // Clear assumptions
					solver.assumeProperty(xorGate.getVar(), true);
					result = solver.assumpSolve();
					if(result==0){
						
						needToUpdate=true;
						merge((*v)[j], (*v)[i], (*v)[j].isInv()^(*v)[i].isInv());
						(*v)[j].setUndef();

					}

					tmpFlag[(*v)[i].gate()->getId()]=true;
					tmpFlag[(*v)[j].gate()->getId()]=true;

				}	
			}
		}
	}
	delete _fecGroup;
	_fecGroup=0;

	// update totalList
	if(needToUpdate){
		updateTotalList();
	}
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/

void CirMgr::genProofModel(SatSolver& s){

	// Allocate and record variables; No Var ID for POs
	for(vector<CirGateV*>::iterator it=totalList.begin();
			it!=totalList.end(); it++){

		if(dynamic_cast<CirAigGate*>((*it)->gate())){

			Var v = s.newVar();
			(*it)->gate()->setVar(v);

		}
	}

	Var v = s.newVar();
	CirAigGate* dummy = new CirAigGate;
	dummy->setVar(v);

	// Hard code the model construction here...
	for(vector<CirGateV*>::iterator it=totalList.begin();
			it!=totalList.end(); it++){

		if(dynamic_cast<CirAigGate*>((*it)->gate())){
		
			CirGate* lhs = (*it)->gate();

			if(lhs->getId()!=0){

				CirGateV* rhs0 = (*it)->gate()->_faninList[0];
				CirGateV* rhs1 = (*it)->gate()->_faninList[1];
				if(!(rhs0->isUndef() || rhs1->isUndef()))
					s.addAigCNF(lhs->getVar(), rhs0->gate()->getVar(), rhs0->isInv(),
							rhs1->gate()->getVar(), rhs1->isInv());
			}
		}
	}
}













