#include "stdafx.h"
#include "any-ticker.h"

/* for errors */
int debugFlag = 0;
int errorNum = 0;

int main( int argc, char *argv[]){
	if( argc > 1){
		if( strcmp( argv[1], "ticker") == 0)
			any_ticker( argc-1, argv+1);
	}else{
		printf("TODO: print help text\n");
	}
	return 0;
}
