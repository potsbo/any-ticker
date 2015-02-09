#include "stdafx.h"
#include "file_manage.h"
extern int debugFlag;

int outputFileInitialise( const char *of, const char *initText){

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( of, "w");

	/* checking the file */
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", of);
		return 1;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", of);
	}

	/* initialisation */
	fprintf( outputFile, "%s", initText);

	/* close */
	fclose( outputFile);

	printf("\nInitialisation done\n");
	return 0;
}

