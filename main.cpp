#include "stdafx.h"
#include "any-ticker.h"
#include <iostream>

using namespace std;

/* for errors */
int debugFlag = 0;
int errorNum = 0;

int main( int argc, char *argv[]){
	if( argc > 1){
		if( strcmp( argv[1], "ticker") == 0)
			any_ticker( argc-1, argv+1);
		else{
			cout << "Error: No command \"" << argv[1] << "\""<< endl;
		}
	}else{
		cout << "TODO: print help text" << endl;
	}
	return 0;
}
