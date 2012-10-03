#include "hw1.2.p2a.h"
#include <iostream>
#include <cstring>
using namespace std;

char* xStr::convertAndPrint(const char *targetStr) const{

	char newOrder[27]="XOBCDAFGHUJKLMNIPQRSTEVWYZ";
										 
	char *returnStr=new char [1];

//allocate returnStr
	for(int i=0; i<strlen(targetStr); i++){
	
		delete [] returnStr;
		returnStr=new char [i+1+1];
	
	}

//convert
	for(int i=0; i<strlen(targetStr); i++){
	
		int charInt = int(targetStr[i]);
	
		if(65 <= charInt && charInt <= 90 ){
		
			for(int j=0; j<26; j++){
			
				if(newOrder[j]==targetStr[i]){
				
					returnStr[i]=j+65;

				}
			}
		}
		else if(97 <= charInt && charInt <= 122 ){
			
			for(int j=0; j<26; j++){
			
				if(newOrder[j]==targetStr[i]-32){
				
					returnStr[i]=j+65;

				}
			}
		}
		
		else{
		
			returnStr[i]=targetStr[i];
		
		}
		
	}

	returnStr[strlen(targetStr)]='\0';
	cout << returnStr << endl;
	return returnStr;

}
