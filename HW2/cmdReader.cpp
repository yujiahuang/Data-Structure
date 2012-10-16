/****************************************************************************
	FileName     [ cmdReader.cpp ]
	PackageName  [ cmd ]
	Synopsis     [ Define command line reader member functions ]
	Author       [ Chung-Yang (Ric) Huang ]
	Copyright    [ Copyleft(c) 2007-2012 LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar checkChar(char, istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
	if (_dofile.is_open()) {
		readCmdInt(_dofile);
		_dofile.close();
	}
	else
		readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
	resetBufAndPrintPrompt();

	while (1) {
		char ch = mygetc(istr);
		ParseChar pch = checkChar(ch, istr);
		if (pch == INPUT_END_KEY) break;
		switch (pch) {
			case LINE_BEGIN_KEY :
			case HOME_KEY       : /* TODO */moveBufPtr(_readBuf); break;
			case LINE_END_KEY   :
			case END_KEY        : /* TODO */moveBufPtr(_readBufEnd); break;
			case BACK_SPACE_KEY : /* TODO */deleteChar(); break;
			case DELETE_KEY     : deleteChar(); break;
			case NEWLINE_KEY    : addHistory();
														cout << char(NEWLINE_KEY);
														printPrompt(); break;
			case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
			case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
			case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr + sizeof(char *)); break;
			case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr - sizeof(char *)); break;
			case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
			case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
			case TAB_KEY        : /* TODO */ break;
			case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
			case UNDEFINED_KEY:   mybeep(); break;
			default:  // printable character
														insertChar(char(pch)); break;
		}
#ifdef TA_KB_SETTING
		taTestOnly();
#endif
	}
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool CmdParser::moveBufPtr(char* const ptr)
{
	// TODO...
	bool returnValue=false;
/*
cout << "//ptr: " << (int)ptr;
cout << "//read buf ptr" << (int)_readBufPtr;
cout << "//read buf " << (int)_readBuf;
cout << "//read buf end" << (int)_readBufEnd;
*/

	if(ptr >= _readBuf && ptr <= _readBufEnd){
	
		if(ptr > _readBufPtr){

			int currentPosi=(_readBufPtr-_readBuf)/sizeof(char *);
			cout << _readBuf[currentPosi];

		}
		else{

			for(unsigned int i=1; i<=(_readBufPtr-ptr)/sizeof(char *); i++) 
				cout << char(BACK_SPACE_CHAR);
	
		}		

		_readBufPtr = ptr;
		returnValue=true;

	}
	else{

		mybeep();
		returnValue = false;

	}

	return returnValue;
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool CmdParser::deleteChar(){
	// TODO...
	bool returnValue=false;
	if(_readBufPtr != _readBufEnd){
	
		returnValue=true;
	
	//renew data
		int currentPosi=(_readBufPtr-_readBuf)/sizeof(char *);
		for(int i=currentPosi; i < strlen(_readBuf)-1; i++){
		
			_readBuf[i]=_readBuf[i+1];
			
		}
		_readBufEnd=&_readBuf[strlen(_readBuf)-1];
		*_readBufEnd=0;

	//	cout << "//"  << _readBuf << "//";	
	//output
		moveBufPtr(_readBuf);
		for(int i=0; i < strlen(_readBuf); i++){

			cout << _readBuf[i];		

		}
		cout << ' ' << char(BACK_SPACE_CHAR);
		for(int i=currentPosi; i < strlen(_readBuf); i++){

			cout << char(BACK_SPACE_CHAR);		

		}

		moveBufPtr(_readBuf + currentPosi*sizeof(char *));
		//cout << "//" << (int)_readBuf + currentPosi*sizeof(char *) - (int)_readBufEnd << "//";
	
	}
	else{
	
		mybeep();
		returnValue=false;
	
	}

	return returnValue;
}

// 1. Insert character 'ch' at _readBufPtr
// 2. Move the remaining string right for one character
// 3. The cursor should move right for one position afterwards
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k') ---
//
// cmd> This is kthe command
//               ^
//
void CmdParser::insertChar(char ch, int rep)
{
	// TODO...

//renew data
	int currentPosi=(_readBufPtr-_readBuf)/sizeof(char *);
	for(int i=strlen(_readBuf); i > currentPosi; i--){

		_readBuf[i]=_readBuf[i-1];

	}
	_readBufEnd += sizeof(char *);
  *_readBufEnd=0;
	_readBuf[currentPosi]=ch;
	_readBufPtr += sizeof(char *);

//output
	int count=0;
	for(unsigned int i=currentPosi; i<strlen(_readBuf); i++){

		cout << _readBuf[i];
		count ++;

	}
	for(int i=1; i<count; i++){

		cout << char(BACK_SPACE_CHAR);

	}

}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
	void
CmdParser::deleteLine()
{
	// TODO...
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
	void
CmdParser::moveToHistory(int index)
{
	// TODO...
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
// 6. Reset _readBufPtr and _readBufEnd to _readBuf
// 7. Make sure *_readBufEnd = 0 ==> _readBuf becomes null string
//
void CmdParser::addHistory(){

	// TODO...
	
	

	_readBufPtr=_readBuf;
	_readBufEnd=_readBuf;
	for(int i=0; i<READ_BUF_SIZE; i++) _readBuf[i]=0;
	*_readBufEnd=0;

}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
	void
CmdParser::retrieveHistory()
{
	deleteLine();
	strcpy(_readBuf, _history[_historyIdx].c_str());
	cout << _readBuf;
	_readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
