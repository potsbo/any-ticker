#include "stdafx.h"
#include "any-ticker.h"
#include <iostream>

using namespace std;

/* for errors */
int debugFlag = 0;
int errorNum = 0;

void help( void);

int main( int argc, char *argv[]){
	if( argc > 1){
		if( strcmp( argv[1], "ticker") == 0)
			any_ticker( argc-1, argv+1);
		else if( strcmp (argv[1], "help")){
			help();
		}else{
			cout << "Error: No command \"" << argv[1] << "\""<< endl;
			help();
		}
	}else{
		help();
	}
	return 0;
}

void help( void){
	cout << "Command list:" << endl;
	cout << "ticker - make a ticker message" << endl;
	cout << "help - show this text" << endl;
}
