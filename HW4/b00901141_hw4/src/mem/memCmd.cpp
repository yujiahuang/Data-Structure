/****************************************************************************
	FileName     [ memCmd.cpp ]
	PackageName  [ mem ]
	Synopsis     [ Define memory test commands ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

	bool
initMemCmd()
{
	if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
				cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
				cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
				cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
			 )) {
		cerr << "Registering \"mem\" commands fails... exiting" << endl;
		return false;
	}
	return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
	CmdExecStatus
MTResetCmd::exec(const string& option)
{
	// check option
	string token;
	if (!CmdExec::lexSingleOption(option, token))
		return CMD_EXEC_ERROR;
	if (token.size()) {
		int b;
		if (!myStr2Int(token, b) || b < int(sizeof(MemTestObj))) {
			cerr << "Illegal block size (" << token << ")!!" << endl;
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
		}
#ifdef MEM_MGR_H
		mtest.reset(toSizeT(b));
#else
		mtest.reset();
#endif // MEM_MGR_H
	}
	else
		mtest.reset();
	return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
	os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
	cout << setw(15) << left << "MTReset: " 
		<< "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
	CmdExecStatus
MTNewCmd::exec(const string& option)
{
	// TODO

	// get options
	vector<string> tokens;
	if(!CmdExec::lexOptions(option, tokens))
		return CMD_EXEC_ERROR;
	
	int num, arr=0;
	
	//error handling
	if(tokens.size()==0){
	
		return CmdExec::errorOption(CMD_OPT_MISSING, "");
	
	}
	else if(tokens.size()==1){
		
		if (!myStr2Int(tokens[0], num) || num<=0) {
	
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);
	
		}
	
	}
	else{
	
		if (!myStr2Int(tokens[0], num) || num<=0) {

			if(tokens[0].length()<=1 || myStrNCmp("-Array", tokens[0], tokens[0].length())!=0){
			
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[0]);

			}
			else if(tokens.size()>3){

				return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[3]);

			}
			else if(!myStr2Int(tokens[1], arr) || arr<=0){
			
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);
			
			}
			else if(tokens.size()==2){

				return CmdExec::errorOption(CMD_OPT_MISSING, "");

			}
			else if(!myStr2Int(tokens[2], num) || num<=0){

				return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);

			}

		}
		else{
			
			if(tokens[1].length()<=1 || myStrNCmp("-Array", tokens[1], tokens[1].length())!=0){

				return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[1]);

			}
			else if(tokens.size()>3){

				return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[3]);

			}
			else if(tokens.size()==2){

				return CmdExec::errorOption(CMD_OPT_MISSING, tokens[1]);

			}
			else if(!myStr2Int(tokens[2], arr) || arr<=0){

				return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);

			}
		
		}

	}

	try {
	
		if(arr==0) mtest.newObjs(num);
		else mtest.newArrs(num, arr);
	
	}catch(bad_alloc ba){

		return CMD_EXEC_ERROR;

	}

	return CMD_EXEC_DONE;

}

void
MTNewCmd::usage(ostream& os) const
{  
	os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
	cout << setw(15) << left << "MTNew: " 
		<< "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus MTDeleteCmd::exec(const string& option){
	// TODO

	// get options
	vector<string> tokens;
	if(!CmdExec::lexOptions(option, tokens))
		return CMD_EXEC_ERROR;
	
	int idx=-1, rnd=0;
	int start=0;
	//error handling

	if(tokens.size()==0){ //make sure # of tokens is >0
	
		return CmdExec::errorOption(CMD_OPT_MISSING, "");
	
	}
	
	if(tokens[0].length()>1 && myStrNCmp("-Array", tokens[0], tokens[0].length())==0){ //Array
	
		start=1;
	
	}
	else{
		
		start=0;
	
	}
	
	if(tokens[start].length()>1 && myStrNCmp("-Index", tokens[start], tokens[start].length())==0){ //Index

		if(tokens.size()<2){ //Missing if options < 2
	
			return CmdExec::errorOption(CMD_OPT_MISSING, tokens[start]);

		}
		else if(!myStr2Int(tokens[start+1], idx) || idx<0){ 
		
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[start+1]);

		}	

	}
	else if(tokens[start].length()>1 && myStrNCmp("-Random", tokens[start], tokens[start].length())==0){
	//Random
	
		if(tokens.size()<2){
	
			return CmdExec::errorOption(CMD_OPT_MISSING, tokens[start]);

		}
		else if(!myStr2Int(tokens[start+1], rnd) || rnd<=0){
		
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[start+1]);

		}
	
	}
	else{
	
		return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[start]);

	}

	if(tokens.size()==2){
	
		if(idx!=-1) {
		
			if(idx >= (signed int)mtest.getObjListSize()){ 
			
				cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " 
						 << idx << "!!" <<endl;
				return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);

			}
			else
				mtest.deleteObj(idx);
		
		}
		else if(rnd!=0) {
		
			if(mtest.getObjListSize()==0){
				
					cerr << "Size of object list is 0!!";
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[1]);

			}
			else{

				for(int i=0; i<rnd; i++){
			
					int rndNum=rnGen(mtest.getObjListSize());
					mtest.deleteObj(rndNum);
			
				}

			}
	
		}

	}
	else if(tokens.size()==3){
	
		if(start==1 || (tokens[2].length()>1 && myStrNCmp("-Array", tokens[2], tokens[2].length())==0)){

			if(idx!=-1){	
			
				if(idx >= (signed int)mtest.getArrListSize()){ 
			
					cerr << "Size of object list (" << mtest.getArrListSize() << ") is <= " 
							 << idx << "!!" << endl;
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[start+1]);

				}
				else
					mtest.deleteArr(idx);
			
			}
			else if(rnd!=0) {
					
				if(mtest.getArrListSize()==0){

					cerr << "Size of array list is 0!!";
					return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[start+1]);

				}
				else{

					for(int i=0; i<rnd; i++){

						int rndNum=rnGen(mtest.getArrListSize());
						mtest.deleteArr(rndNum);
					
					}

				}

			}

		}
		else{
	
			return CmdExec::errorOption(CMD_OPT_ILLEGAL, tokens[2]);
		
		}

	}
	else{
	
		return CmdExec::errorOption(CMD_OPT_EXTRA, tokens[3]);
	
	}

	return CMD_EXEC_DONE;

}

void
MTDeleteCmd::usage(ostream& os) const
{  
	os << "Usage: MTDelete <-Index (size_t objId) | "
		<< "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
	cout << setw(15) << left << "MTDelete: " 
		<< "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
	CmdExecStatus
MTPrintCmd::exec(const string& option)
{
	// check option
	if (option.size())
		return CmdExec::errorOption(CMD_OPT_EXTRA, option);
	mtest.print();

	return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
	os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
	cout << setw(15) << left << "MTPrint: " 
		<< "(memory test) print memory manager info" << endl;
}


