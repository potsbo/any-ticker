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
	/* int yGunCentre = 292; */
	int yGunPart1Centre = 88;
	/* int xGunCentre = -5; //Can't be closer than this */
	int xGunPart1Centre = -5; //Can't be closer than this
	int yGunPart2Centre = 85;
	int xGunPart2Centre = -22; //Can't be closer than this
	
	char eaterFileName[100];
	/* set file name here */
	/* strcpy( eaterFileName, "ticker.eater.life"); */
	strcpy( eaterFileName, "eater.life");

	char outputFileName[100];
	/* set file name here */	
	strcpy( outputFileName, "any-ticker.life");
	outputFileInitialise( outputFileName);

	char gunPart1FileName[100];
	/* set file name here */
	strcpy( gunPart1FileName, "ticker.gun.part1.life");

	char gunPart2FileName[100];
	/* set file name here */
	strcpy( gunPart2FileName, "ticker.gun.part2.life");
	
	char gliderPhase1FileName[100];
	/* set file name here */
	strcpy( gliderPhase1FileName, "glider.phase1.life");
	int xGlider1Centre = -23;
	int yGlider1Centre = 68;

	int gunNum = 1;
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1,i );
		if( append( gunPart1FileName, outputFileName, -xGunPart1Centre - 115 * (i/2) , -yGunPart1Centre - 18 * (i/2), yFlag) != 0 ){ 
			printf("Error in function \"append\". abort\n");
			return 1;
		}
		if( append( gunPart2FileName, outputFileName, -xGunPart2Centre - 115 * (i/2) , -yGunPart2Centre - 18 * (i/2), yFlag) != 0 ){ 
			printf("Error in function \"append\". abort\n");
			return 1;
		}
		if( append( gliderPhase1FileName, outputFileName, -xGlider1Centre - 115 * (i/2) , -yGlider1Centre - 18 * (i/2), yFlag) != 0 ){ 
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
		int negFlag = pow( -1, (i + 2)/2);
		if( append( eaterFileName, outputFileName, -xEaterCentre - distant, -yEaterCentre - negFlag * 36 * ( (i + 2)/4), yFlag) != 0 ){ 
			printf("Error in function \"append\". abort\n");
			return 1;
		}
	}
	printf("%d eaterss are put\n", eaterNum);

	printf("\nEnd combining with no error\n");
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
