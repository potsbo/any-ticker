#include <stdio.h>
#include <string.h>
#include <math.h>

int append( char *inputFileName, char *outputFileName, int shiftX, int shiftY, int yDirection);
int outputFileInitialise( char *outputFileName);
int debugFlag = 0;

int main(int argc, char *argv[]){
	int yEatersCentre = 79;
	int xEatersCentre = 18;
	int yEaterCentre = -11;
	int xEaterCentre = 4;
	int yGunCentre = 292;
	int xGunCentre = -5; //Can't be closer than this
	
	char eaterFileName[100];
	/* set file name here */
	/* strcpy( eaterFileName, "ticker.eater.life"); */
	strcpy( eaterFileName, "eater.life");

	char outputFileName[100];
	/* set file name here */	
	strcpy( outputFileName, "any-ticker.life");
	FILE *outputFile;
	outputFileInitialise( outputFileName);

	char gunFileName[100];
	/* set file name here */
	strcpy( gunFileName, "ticker.gun.life");
	
	int gunNum = 9;
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1,i );
		if( append( gunFileName, outputFileName, -xGunCentre - 115 * (i/2) , -yGunCentre - 18 * (i/2), yFlag) != 0 ){ 
			printf("Error in function \"append\". abort\n");
			return 1;
		}

	}
	printf("%d gun(s) put\n", gunNum);

	/* putting eaters */
	int distant = 0;
	distant += 115 *  ( (gunNum - 1) / 2) - ((gunNum -1) / 2) % 2;
	int eaterNum = gunNum + 2;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag =1;
		if((i+3) % 4 == 0 || i % 4 == 0){ negFlag = -1; }
		if( append( eaterFileName, outputFileName, -xEaterCentre - distant, -yEaterCentre - negFlag * 36 * ( (i + 2)/4), yFlag) != 0 ){ 
			printf("Error in function \"append\". abort\n");
			return 1;
		}
	}
	/* if( append( eaterFileName, outputFileName, -xEatersCentre - distant, -yEatersCentre, 1) != 0 ){ */ 
	/* 	printf("Error in function \"append\". abort\n"); */
	/* 	return 1; */
	/* } */
	printf("Eaters are put\n");

	printf("End combining with no error\n");
}

int outputFileInitialise( char *outputFileName){
	FILE *outputFile;
	outputFile = fopen( outputFileName, "w");
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", outputFileName);
		return 1;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", outputFileName);
	}
	fprintf( outputFile, "#Life 1.06\n");
	fclose( outputFile);

	printf("Initialisation done\n");
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
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", inputFileName );
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( outputFileName, "a");
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", outputFileName);
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", outputFileName);
	}

	if(debugFlag != 0)printf("start reading %s\n", inputFileName);
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
	if( debugFlag != 0) printf("end reading \"%s\"\n", inputFileName);
	fclose( inputFile);
	fclose( outputFile);

	return 0;
}
