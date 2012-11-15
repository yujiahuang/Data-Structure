/****************************************************************************
	FileName     [ cmdParser.cpp ]
	PackageName  [ cmd ]
	Synopsis     [ Define command parsing member functions for class CmdParser ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Global cmd Manager
//----------------------------------------------------------------------
CmdParser* cmdMgr = new CmdParser("mcalc> ");


//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();


//----------------------------------------------------------------------
//    Member Function for class cmdParser
//----------------------------------------------------------------------
// return false if file cannot be opened
// Please refer to the comments in "DofileCmd::exec", cmdCommon.cpp
bool CmdParser::openDofile(const string& dof){

	// TODO...
	bool returnedValue=false;

	if(_dofile){

		if(_dofileStack.size()>1024) return false;

		_dofileStack.push(_dofile);
		_dofile=new ifstream(dof.c_str());

		if(!_dofile->fail()){
		
			cmdMgr->execOneCmd();
			returnedValue = true;
		
		}
		else{

			delete _dofile;
			_dofile = 0;

			returnedValue = false;
	
		}

		if(_dofileStack.size()!=0){
			
			_dofile = _dofileStack.top();
			_dofileStack.pop();
		
		}

	}
	else{
	
		_dofile = new ifstream(dof.c_str());

		if(!_dofile->fail()){
		
			returnedValue=true;

		}
		else{
	
			delete _dofile;
			_dofile = 0;

			returnedValue = false;
		
		}

	}


	return returnedValue;

}

// Must make sure _dofile != 0
void CmdParser::closeDofile(){

	assert(_dofile != 0);
	// TODO...

	//cout << "//closing" << _dofile << "//";

	_dofile -> close();
	_dofile -> clear();

	delete _dofile;
	_dofile=0;

}

// Return false if registration fails
	bool
CmdParser::regCmd(const string& cmd, unsigned nCmp, CmdExec* e)
{
	// Make sure cmd hasn't been registered and won't cause ambiguity
	string str = cmd;
	unsigned s = str.size();
	if (s < nCmp) return false;
	while (true) {
		if (getCmd(str)) return false;
		if (s == nCmp) break;
		str.resize(--s);
	}

	// Change the first nCmp characters to upper case to facilitate
	//    case-insensitive comparison later.
	// The strings stored in _cmdMap are all upper case
	//
	assert(str.size() == nCmp);  // str is now mandCmd
	string& mandCmd = str;
	for (unsigned i = 0; i < nCmp; ++i)
		mandCmd[i] = toupper(mandCmd[i]);
	string optCmd = cmd.substr(nCmp);
	assert(e != 0);
	e->setOptCmd(optCmd);

	// insert (mandCmd, e) to _cmdMap; return false if insertion fails.
	return (_cmdMap.insert(CmdRegPair(mandCmd, e))).second;
}


// Return false on "quit" or if excetion happens
	CmdExecStatus
CmdParser::execOneCmd()
{
	bool newCmd = false;
	if (_dofile != 0)
		newCmd = readCmd(*_dofile);
	else
		newCmd = readCmd(cin);

	// execute the command
	if (newCmd) {
		string option;
		CmdExec* e = parseCmd(option);
		if (e != 0)
			return e->exec(option);
	}
	return CMD_EXEC_NOP;
}

// For each CmdExec* in _cmdMap, call its "help()" to print out the help msg.
// Print an endl at the end.
void CmdParser::printHelps() const{
	// TODO...

	CmdMap::const_iterator it=_cmdMap.begin();

	for( ; it!=_cmdMap.end(); it++){

		(*(*it).second).help();	

	}

	cout << endl;

}

void CmdParser::printHistory(int nPrint) const{

	assert(_tempCmdStored == false);
	if (_history.empty()) {
		cout << "Empty command history!!" << endl;
		return;
	}
	int s = _history.size();
	if ((nPrint < 0) || (nPrint > s))
		nPrint = s;
	for (int i = s - nPrint; i < s; ++i)
		cout << "   " << i << ": " << _history[i] << endl;

}


//
// Parse the command from _history.back();
// Let string str = _history.back();
//
// 1. Read the command string (may contain multiple words) from the leading
//    part of str (i.e. the first word) and retrive the corresponding
//    CmdExec* from _cmdMap
//    ==> If command not found, print to cerr the following message:
//        Illegal command!! "(string cmdName)"
//    ==> return it at the end.
// 2. Call getCmd(cmd) to retrieve command from _cmdMap.
//    "cmd" is the first word of "str".
// 3. Get the command options from the trailing part of str (i.e. second
//    words and beyond) and store them in "option"
//
CmdExec* CmdParser::parseCmd(string& option){

	assert(_tempCmdStored == false);
	assert(!_history.empty());
	string str = _history.back();

	// TODO...
	string token;
	myStrGetTok(str, token);	

	CmdExec* cmd = getCmd(token);

	if(cmd){

		option=str.substr(token.length());

	}
	else{

		cout << "Illegal command!! (" << token << ")" << endl;

	}


	assert(str[0] != 0 && str[0] != ' ');
	return cmd;

}

// This function is called by pressing 'Tab'.
// It is to list the partially matched commands.
// "str" is the partial string before current cursor position. It can be 
// a null string, or begin with ' '. The beginning ' ' will be ignored.
//
// Several possibilities after pressing 'Tab'
// (Let $ be the cursor position)
// 1. [Before] Null cmd
//    cmd> $
//    -----------
//    [Before] Cmd with ' ' only
//    cmd>     $
//    [After Tab]
//    ==> List all the commands, each command is printed out by:
//           cout << setw(12) << left << cmd;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location (including ' ')
//
// 2. [Before] partially matched (multiple matches)
//    cmd> h$                   // partially matched
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$                   // and then re-print the partial command
//    -----------
//    [Before] partially matched (multiple matches)
//    cmd> h$aaa                // partially matched with trailing characters
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$aaa                // and then re-print the partial command
//
// 3. [Before] partially matched (single match)
//    cmd> he$
//    [After Tab]
//    cmd> heLp $
//    -----------
//    [Before] partially matched with trailing characters (single match)
//    cmd> he$hahah
//    [After Tab]
//    cmd> heLp $hahaha
//    ==> Automatically complete on the same line
//    ==> The auto-expanded part follow the strings stored in cmd map and
//        cmd->_optCmd. Insert a space after "heLp"
//
// 4. [Before] No match
//    cmd> hek$
//    [After Tab]
//    ==> Beep and stay in the same location
//
// 5. [Before] Already matched
//    cmd> help asd$fg
//    [After] Print out the usage for the already matched command
//    Usage: HELp [(string cmd)]
//    cmd> help asd$fg
//
// 6. [Before] Cursor NOT on the first word and NOT matched command
//    cmd> he haha$kk
//    [After Tab]
//    ==> Beep and stay in the same location
//
void CmdParser::listCmd(const string& str){

	// TODO...
	string inputStr = str.substr(0, _readBufPtr-_readBuf);

	CmdMap::iterator it=_cmdMap.begin();
	CmdMap foundMap;
	bool match=false;

	//iterate thru cmdMap
	for(int count=0; it!=_cmdMap.end(); it++, count++){

		string compared=(*it).first;
		
		//check if is on first word
		if(_readBufPtr!=_readBuf){
		
			bool found = false;
			bool remain=false;

			//check if there is remaining chars
			if(compared.length()<inputStr.length()) remain=true;
			else remain=false;

			//check if found or match

			//cout << "//" << inputStr.length() << "//" << compared.length() << "//";
			if(inputStr.length()>0 && compared.length()<=inputStr.length() && myStrNCmp(inputStr, compared, compared.length())==0){

				found=true;
				match=true;

			}
			else if(inputStr.length()>0 && compared.length()>inputStr.length() && myStrNCmp(compared, inputStr, inputStr.length())==0){

				found=true;

			}

			//cout << "//found checked//";

			//if match, no need to check more
			if(match){

				CmdExec *e;
				e=(*it).second;
				cout << endl;
				e->usage(cout);
				reprintCmd();
				break;

			}

			//check optional
			if(found){

				CmdExec *e;
				e=(*it).second;
				bool optCmdCorrect=false;
				
				if(remain){
					
					string remaindar=inputStr.substr((*it).first.length());
					optCmdCorrect=e->checkOptCmd(remaindar);

				}

				if( optCmdCorrect || !remain){

					foundMap.insert( CmdRegPair( (*it).first, (*it).second ) );

				}

			}

		}
		else{
	
			if(count%5==0) cout << endl; //print empty line
			string cmd;
			cmd.append(compared);
			cmd.append((*it).second->getOptCmd());
			cout << setw(12) << left << cmd; //print cmd
		
		}

		//cout << "//found//" << found << "//match//" << match << "//remain//" << remain;

	} //iteration end

	//check found number
	if(_readBufPtr!=_readBuf){ //'==' situation aready handled

		if(foundMap.size()==0) mybeep();
		else if(foundMap.size()==1 && !match){ //'match' situaltion already handled
		
			string outputStr;
			outputStr.append(foundMap.begin()->first);
			outputStr.append(foundMap.begin()->second->getOptCmd());
			
			string outputSub=outputStr.substr(_readBufPtr-_readBuf);

			//handle _readBuf stuff
			for(int i=0; i<(signed int)outputSub.length(); i++){
			
				insertChar(outputSub.c_str()[i]);
			
			}
			insertChar(' ');
			_readBufPtr+=outputSub.length();
			_readBufEnd+=outputSub.length();
	
		} 
		else if(foundMap.size()>1){
	
			cout << endl;
			for(CmdMap::iterator i=foundMap.begin(); i!=foundMap.end(); i++){
		
				string output;
				output.append((*i).first);
				output.append((*i).second->getOptCmd());
				cout << setw(12) << left << output;
			
			}
			reprintCmd();
		
		}
	
	}
	else{
	
		reprintCmd();

	}


}

// cmd is a copy of the original input
//
// return the corresponding CmdExec* if "cmd" matches any command in _cmdMap
// return 0 if not found.
//
// Please note:
// ------------
// 1. The mandatory part of the command string (stored in _cmdMap) must match
// 2. The optional part can be partially omitted.
//    ==> Checked by the CmdExec::checkOptCmd(const string&) function
// 3. All string comparison are "case-insensitive".
//
CmdExec* CmdParser::getCmd(string cmd){

	CmdExec* e = 0;
	// TODO...
	// call CmdExec::checkOptCmd(const string&) if needed...

	//search
	CmdMap::iterator it=_cmdMap.begin();
	bool found=false;
	bool remain=false;

	for(; it!=_cmdMap.end() && !found; it++){

		string compared=(*it).first;

		//check if there is remaining chars
		if(compared.length()<cmd.length()) remain=true;
		else remain=false;

		//check if match
		if(compared.length()<=cmd.length() && myStrNCmp(cmd, compared, compared.length())==0) found=true;

	}

	//check optional
	if(found){

		it--;
		e=(*it).second;
		if(remain){

			string remaindar=cmd.substr((*it).first.length());
			if(!e->checkOptCmd(remaindar)) e=0;

		}

	}

	return e;

}


//----------------------------------------------------------------------
//    Member Function for class CmdExec
//----------------------------------------------------------------------
// Return false if error options found
// "optional" = true if the option is optional XD
// "optional": default = true
//
bool
CmdExec::lexSingleOption
(const string& option, string& token, bool optional) const
{
	size_t n = myStrGetTok(option, token);
	if (!optional) {
		if (token.size() == 0) {
			errorOption(CMD_OPT_MISSING, "");
			return false;
		}
	}
	if (n != string::npos) {
		errorOption(CMD_OPT_EXTRA, option.substr(n));
		return false;
	}
	return true;
}

// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool
CmdExec::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
	string token;
	size_t n = myStrGetTok(option, token);
	while (token.size()) {
		tokens.push_back(token);
		n = myStrGetTok(option, token, n);
	}
	if (nOpts != 0) {
		if (tokens.size() < nOpts) {
			errorOption(CMD_OPT_MISSING, "");
			return false;
		}
		if (tokens.size() > nOpts) {
			errorOption(CMD_OPT_EXTRA, tokens[nOpts]);
			return false;
		}
	}
	return true;
}

CmdExecStatus
CmdExec::errorOption(CmdOptionError err, const string& opt) const
{
	switch (err) {
		case CMD_OPT_MISSING:
			cerr << "Missing option";
			if (opt.size()) cerr << " after (" << opt << ")";
			cerr << "!!" << endl;
			break;
		case CMD_OPT_EXTRA:
			cerr << "Extra option!! (" << opt << ")" << endl;
			break;
		case CMD_OPT_ILLEGAL:
			cerr << "Illegal option!! (" << opt << ")" << endl;
			break;
		case CMD_OPT_FOPEN_FAIL:
			cerr << "Error: cannot open file \"" << opt << "\"!!" << endl;
			break;
		default:
			cerr << "Unknown option error type!! (" << err << ")" << endl;
			exit(-1);
	}
	return CMD_EXEC_ERROR;
}

// Called by "getCmd()"
// Check if "check" is a matched substring of "_optCmd"...
// if not, return false.
// 
// Perform case-insensitive checks
//
bool
CmdExec::checkOptCmd(const string& check) const
{
	// TODO...

	bool returnedValue=false;
	if(check.length()<=_optCmd.length()) returnedValue = (myStrNCmp(_optCmd, check, check.length())==0) ? true : false;

	return returnedValue;
}