#include <stdio.h>
#include <string.h>

int append( char *inputFileName, char *outputFileName, int shiftX, int shiftY, int yDirection);
int outputFileInitialise( char *outputFileName);

int main(int argc, char *argv[]){
	int yEaterCentre = 97;
	int xEaterCentre = 18;
	int yGunCentre = 292;
	int xGunCentre = -6; //Can't be closer than this
	
	char eaterFileName[100];
	/* set file name here */
	strcpy( eaterFileName, "ticker.eater.life");

	char outputFileName[100];
	/* set file name here */	
	strcpy( outputFileName, "any-ticker.life");
	FILE *outputFile;
	outputFileInitialise( outputFileName);

	/* putting eaters */
	if( append( eaterFileName, outputFileName, -xEaterCentre, -yEaterCentre, 1) != 0 ){ 
		printf("Error in function \"append\". abort\n");
		return 1;
	}

	char gunFileName[100];
	/* set file name here */
	strcpy( gunFileName, "ticker.gun.life");
	
	/* putting a gun */
	if( append( gunFileName, outputFileName, -xGunCentre, -yGunCentre, 1) != 0 ){ 
		printf("Error in function \"append\". abort\n");
		return 1;
	}
	if( append( gunFileName, outputFileName, -xGunCentre, -yGunCentre, -1) != 0 ){ 
		printf("Error in function \"append\". abort\n");
		return 1;
	}
}

int outputFileInitialise( char *outputFileName){
	FILE *outputFile;
	outputFile = fopen( outputFileName, "w");
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", outputFileName);
		return 1;
	}else{
		printf("\"%s\" is successfully created\n", outputFileName);
	}
	fprintf( outputFile, "#Life 1.06\n");
	fclose( outputFile);

	return 0;
}

int append( char *inputFileName, char *outputFileName, int shiftX, int shiftY, int yDirection){
	
	/* opening input file */
	FILE *inputFile;
	inputFile = fopen( inputFileName, "r");
	if( inputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", inputFileName );
		return 1;
	}else{
		printf("\"%s\" is successfully opened\n", inputFileName );
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( outputFileName, "a");
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", outputFileName);
		return 1;
	}else{
		printf("\"%s\" is successfully opened\n", outputFileName);
	}

	printf("start reading %s\n", inputFileName);
	char tempString[1000];
	tempString[0] = '#';
	while( tempString[0] == '#') fgets( tempString, sizeof(tempString), inputFile);
	/* printf("%s\n", tempString); */
	
	int eofFlag =0;
	while( eofFlag != 1){
		int xTemp, yTemp;
		sscanf( tempString, "%d %d", &xTemp, &yTemp);
		fprintf( outputFile, "%d %d\n", xTemp + shiftX, (yTemp + shiftY )*yDirection);
		if( fgets( tempString, sizeof(tempString), inputFile) == NULL){
			eofFlag = 1;
		}else{
			/* printf("%s", tempString); */
		}
	}
	printf("end reading \"%s\"\n", inputFileName);
	fclose( inputFile);
	fclose( outputFile);

	return 0;
}
