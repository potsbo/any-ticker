#include "stdafx.h"
#include "file_manage.h"
extern int debugFlag;
using namespace std;

void LifeFile::init( string initText){

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( fileName.c_str(), "w");

	/* checking the file */
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", fileName.c_str());
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", fileName.c_str());
	}

	/* initialisation */
	fprintf( outputFile, "%s", initText.c_str());

	/* close */
	fclose( outputFile);

	cout << "\nInitialisation done\n";
}
