/****************************************************************************
	FileName     [ cirSim.cpp ]
	PackageName  [ cir ]
	Synopsis     [ Define cir simulation functions ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2008-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <cmath>
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

/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void CirMgr::randomSim(){

	srand(1);

	size_t s=input.size();
	size_t noChange=0;
	size_t lastFecN=0;
	size_t num=0;

	while(noChange < log10(input.size())/0.3 ){

		unsigned int* sequenceI = new unsigned int [s];
		for(size_t i=0; i<s; i++){

			sequenceI[i] = rand();

		}

		simulation(sequenceI, s, 32);	
		if(_fecGroup==0) createFecGroup(32);
		else continueGroup(32);

		size_t thisFecN = _fecGroup->size();
		if(lastFecN==thisFecN) ++noChange;
		else noChange=0;
		lastFecN=thisFecN;
		if(_fecGroup!=0 && lastFecN!=0) cout << "\nTotal #FEC Group = " << lastFecN/2 << endl;
		
		num++;

	}
	cout << "\n" << 32*num << " patterns simulated." << endl;

}

size_t CirMgr::trimStr(string& line){

	for(string::iterator it=line.begin(); it!=line.end(); it++){

		if((*it)==' ') line.erase(it);
		else break;

	}
	for(string::iterator it=line.end()-1; it!=line.begin(); it--){

		if((*it)==' ') line.erase(it);
		else break;

	}
	return line.size();

}

void CirMgr::fileSim(ifstream& patternFile){

	_simNum=0;
	string line;
  getline(patternFile, line);
	size_t s=trimStr(line);
	if(s==0) getline(patternFile, line);
	vector<char>* sequence=new vector<char> [s];
	bool stop=false;

	do{

		s=trimStr(line);	

		if(s==0) continue;
		if(s!=input.size()){

			printErrorMsg(line, 0, 0);
			_simNum=0;
			break;

		}		

		_simNum++;
		for(size_t i=0; i < s; i++){
			
			if(line[i]=='0' || line[i]=='1')
				sequence[i].push_back(line[i]);
			else{

				printErrorMsg(line, i, 1);
				_simNum=0;
				stop=true;
				break;

			}
		}	

	}while(getline(patternFile, line) && !stop);

	if(_simNum!=0){

		// convert to unsigned int
		unsigned int* sequenceI = new unsigned int [s];
		size_t i=0;
		for(; i<_simNum; i+=32){

			for(size_t k=0; k < s; k++) sequenceI[k]=0;
			size_t k=i;
			for(; k<i+32 && k<_simNum; k++){

				for(size_t j=0; j<s; j++){

					sequenceI[j] += (unsigned int)(sequence[j][k]-48)*pow(2, k);

				}
			}
			simulation(sequenceI, s, k-i);	
			if(_fecGroup==0) createFecGroup(k-i);
			else continueGroup(k-i);
			if(_fecGroup!=0 && _fecGroup->size()!=0) cout << "\nTotal #FEC Group = " << _fecGroup->size()/2;
		}

	}
	else{

		sequence->clear();

	}

	cout << "\n" << _simNum << " patterns simulated." << endl;

}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/

void CirMgr::printErrorMsg(string line, size_t errorPoint, size_t type){

	switch(type){
		case 0: 
			cerr << "Error: Pattern(" << line 
				<< ") length(" 
				<< line.size() << ") does not match the number of inputs(" 
				<< input.size() << ") in a circuit!!" << endl;
			break;
		case 1:
			cerr << "Error: Pattern(" << line 
				<< ") contains a non-0/1 character('" 
				<< line[errorPoint] << "')." << endl;
			break;

	}
}

void CirMgr::simulation(unsigned int* sequence, size_t size, size_t length){

	simulationValue=new unsigned int[M+1];
	for(size_t i=0; i<M+1; i++) simulationValue[i]=0;

	for(size_t i=0; i<input.size(); i++){

		CirGate* gate = input[i]->gate();
		simulationValue[gate->getId()]=sequence[i];

	}

	for(size_t power=0; power<length; power++){

		unsigned int enzyme = pow(2, power);

		for(size_t k=0; k<totalList.size(); k++){

			CirGate* gate = totalList[k]->gate();
			if(dynamic_cast<CirAigGate*>(gate)){

				size_t id = gate->getId();
				if(id==0)continue;
				int faninValue[2]={0, 0};
				for(int i=0; i<2; i++){

					faninValue[i]=(simulationValue[gate->_faninList[i]->gate()->getId()]/enzyme)%2;
					if(gate->_faninList[i]->isInv()) faninValue[i]=!faninValue[i];

				}
				int result = faninValue[0] & faninValue[1];
				simulationValue[id]+=result*pow(2, power);

			}
		}
	}
	if(_simLog!=0) writeLogFile(sequence, size, simulationValue, M+1, length);

}

void CirMgr::writeLogFile(unsigned int* inputSeq, size_t inputSize, 
	unsigned int* outputSeq, size_t outputSize, size_t length){

	for(size_t i=0; i<length; i++){
	
		unsigned int enzyme = pow(2, i);
		for(size_t j=0; j<input.size(); j++)
			(*_simLog) << (inputSeq[j]/enzyme)%2;
		(*_simLog) << " ";
		for(vector<CirGateV*>::iterator it=output.begin();
				it!=output.end(); it++){
		
			CirGateV* gateV = (*it)->gate()->_faninList[0];
			bool result = (outputSeq[gateV->gate()->getId()]/enzyme)%2;
			if(gateV->isInv()) (*_simLog) << !result;
			else (*_simLog) << result;

		}
		(*_simLog) << endl;

	}

}

void CirMgr::createFecGroup(size_t maxSimNum){

	_fecGroup = new vector< vector<CirGateV>* >;
	vector<CirGateV>* aGroup=new vector<CirGateV>;
	for(vector<CirGateV*>::iterator it=totalList.begin();
			it!=totalList.end(); it++){

		CirGate* gate = (*it)->gate();
		if(dynamic_cast<CirAigGate*>(gate)){

			CirGateV gv=*(*it);
			if(gv.isInv()) gv.resetInv();
			else gv.setInv();
			aGroup->push_back(*(*it));
			aGroup->push_back(gv);

		}
	}

	// create const 0
	CirAigGate *const0G = new CirAigGate( 0 );
	CirGateV *const0 = new CirGateV(const0G, 0);
	CirGateV *const1 = new CirGateV(const0G, 1);
	aGroup->push_back(*const0);
	aGroup->push_back(*const1);

	group(aGroup, 0, maxSimNum);

}

void CirMgr::continueGroup(size_t maxSimNum){

	size_t i=0;
	size_t s=_fecGroup->size();
	vector<CirGateV>** oldFec = new vector<CirGateV>* [s];
	for(vector< vector<CirGateV>* >::iterator it = _fecGroup->begin();
			it!=_fecGroup->end(); it++){
	
		oldFec[i]=(*it);
		i++;
			
	}

	for(size_t j=0; j<s; j++){

		vector<CirGateV> *v = oldFec[j];
		group(v, 0, maxSimNum);

	}

}

void CirMgr::group(vector<CirGateV>* &aGroup, size_t depth, size_t maxSimNum){

	//cout << "depth: " << depth << endl << "  ";
	if(_fecGroup==0) return;
	if(depth < maxSimNum){

		vector<CirGateV>* ones=new vector<CirGateV>;
		vector<CirGateV>* zeros=new vector<CirGateV>;
		for(vector<CirGateV>::iterator it=aGroup->begin();
				it!=aGroup->end(); it++){

			unsigned int enzyme=pow(2, depth);
			if((*it).isInv()){

				if((*it).gate()->getId()==0) ones->push_back((*it));
				else if((simulationValue[(*it).gate()->getId()]/enzyme)%2==0){ ones->push_back(*it);}
				else{ zeros->push_back(*it);}

			}
			else{
			
				if((*it).gate()->getId()==0) zeros->push_back((*it));
				else if((simulationValue[(*it).gate()->getId()]/enzyme)%2==0){ zeros->push_back(*it);}
				else{ ones->push_back(*it);}

			}
		}

		bool onesAlive = (ones->size()>1);
		bool zerosAlive = (zeros->size()>1);	
		
		/*
		cout << "depth: " << depth << endl << "  ";
		for(vector<CirGateV>::iterator it = (*zeros).begin(); it!=(*zeros).end(); it++)
			cout << (*it).gate()->getId() << " ";
		cout << "\n  ";
		for(vector<CirGateV>::iterator it = (*ones).begin(); it!=(*ones).end(); it++)
			cout << (*it).gate()->getId() << " ";
		cout << endl;
		*/

		if(zerosAlive && onesAlive){

			delete aGroup;
			removeFromFecGroup(aGroup);
			group(zeros, depth+1, maxSimNum);
			group(ones, depth+1, maxSimNum);

		}else if(zerosAlive && !onesAlive){		

			delete aGroup;
			removeFromFecGroup(aGroup);
			group(zeros, depth+1, maxSimNum);

		}else if(!zerosAlive && onesAlive){

			delete aGroup;
			removeFromFecGroup(aGroup);
			group(ones, depth+1, maxSimNum);

		}else{

			delete aGroup;
			removeFromFecGroup(aGroup);

		}
	}
	else {

		_fecGroup->push_back(aGroup);

	}
}

void CirMgr::removeFromFecGroup(vector<CirGateV>* aGroup){

	for(vector< vector<CirGateV>* >::iterator it = _fecGroup->begin();
			it!=_fecGroup->end(); it++){

		if((*it)==aGroup) _fecGroup->erase(it);

	}

}

