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
void
CirGate::reportGate() const
{
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

void CirAigGate::printGate() const{

	cout << "AIG " << _id; 
	for(int i=0; i<=1; i++){

		//TODO need to consider undef
		cout << (_faninList[i]->isInv() ? " !" : " ");
		cout << _faninList[i]->gate()->getId();

	}

}

void CirPiGate::printGate() const{

	cout << "PI  " << _id;
	if(_name.size()!=0) cout << " (" << _name << ")";
	//TODO need to consider undef

}

void CirPoGate::printGate() const{

	//TODO need to consider undef
	cout << "PO  " << _id; 
	cout << (_faninList[0]->isInv() ? " !" : " ");
	cout << _faninList[0]->gate()->getId();
	if(_name.size()!=0) cout << " (" << _name << ")";

}
