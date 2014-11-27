#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int outputFileInitialise( char *of);
int debugFlag = 0;
int errorNum = 0;
char *iPathPrefix ="./objects/";

typedef struct {
	char fileNameRoot[100];
	/* filename must be "fileNameRoot.phase.direction.life" */
	int xCentre;
	int yCentre;
	int phase;
	int direction;
	// 0: no move // 1: South West // 2: South East // 3: North East // 4: North West
} object;

int append( object type, char *of, int shiftX, int shiftY, int yDirection);

int main(int argc, char *argv[]){
	
	int xLeastAreaSize = 41;
	int yLeastAreaSize = 9;
	double bannerSize = 1.5;

	/* settign objects */
	object eat = {"eater", 4, -11, 0, 0};
	object gun = {"gun", -5, 88, 0, 0};
	object ref = {"reflector", -22, 85, 0, 0,};
	object g01 = {"glider", -23, 68, 0, 1};
	object g12 = {"glider", -28, 53, 1, 2};
	object g14 = {"glider", -47, 57, 1, 4};
	object g33 = {"glider", -36, 69, 3, 3};
	object g13 = {"glider", -39, 45, 1, 3};
	object g21 = {"glider", -35, 79, 2, 1};
	object g23 = {"glider", -50, 57, 2, 3};

	/* setting output file */
	char of[100];
	/* set file name here */	
	strcpy( of, "any-ticker.life");
	outputFileInitialise( of);

	/* calculating area size; xArea should be 4n + 5(n >= 0) */
	int xAreaSize = 5;
	int refShift= 0;
	while( xAreaSize < xLeastAreaSize){
		xAreaSize += 4;
		refShift++;
	}

	/* setting the dot map */
	/* unsigned long long int string = 0b10010101110101110101110111110101111101110; */
	unsigned long long int string = 0b11111111111111111111111111111111111111111;
	int dots[64][1000];
	for(int i = 0; i < xAreaSize; i++){
		dots[0][xAreaSize - 1 - i] = string % 2;
		string /= 0b10;
	}


	/* putting guns, reflectors, and gliders */
	int gunNum = yLeastAreaSize;
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i);
		int xShift = 115 *(i/2);
		int yShift = 18 * (i/2);

		/* gun */
		append( gun, of, -xShift, -yShift, yFlag);
		/* reflector */
		append( ref, of, - xShift +23*refShift, -yShift -23*refShift, yFlag);

		/* gliders */
		for( int i = 0; i < refShift +1; i++){
			if( dots[0][i*2] == 1)
				append( g01, of, -xShift + 23*i, -yShift -23*i, yFlag);
			if( dots[0][xAreaSize - 2 - 2*i] == 1)
				append( g13, of, -xShift +23*i, -yShift -23*i, yFlag);
		}

		for( int i = 0; i < refShift; i++){
			if( dots[0][i*2 + 1] == 1)
				append( g21, of, -xShift +23*i, -yShift -23*i, yFlag);
			if( dots[0][xAreaSize - 3 - 2*i] == 1)
				append( g23, of, -xShift +23*i, -yShift -23*i, yFlag);
		}

		if( dots[0][ 5 -1 +4*refShift])
			append( g12, of, -xShift , - yShift, yFlag);
		if( dots[0][ 3 +2*refShift] == 1)
			append( g14, of, -xShift + 23*refShift, -yShift -23*refShift, yFlag);
		if( dots[0][ 4 +2*refShift -1] == 1)
			append( g33, of, -xShift + 23*refShift, -yShift -23*refShift, yFlag);

	}
	printf("%d gun(s) put\n", gunNum);

	/* putting eaters */
	int distant = 0;
	distant += (int)ceil((xAreaSize *23) *abs(bannerSize) /2) *2;
	distant += 115 *  ((gunNum -1) /2) - ((gunNum -1) /2) % 2;
	int eaterNum = gunNum + 2;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag = pow( -1, (i + 2)/2);
		append( eat, of, - distant, - negFlag * 36 * ( (i + 2)/4), yFlag);
	}
	printf("%d eaters are put\n", eaterNum);

	printf("\nEnd combining with %d error(s)\n", errorNum);
}

int outputFileInitialise( char *of){
	FILE *outputFile;
	outputFile = fopen( of, "w");
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", of);
		return 1;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", of);
	}
	fprintf( outputFile, "#Life 1.06\n");
	fclose( outputFile);

	printf("Initialisation done\n\n");
	return 0;
}

int append( object type, char *of, int shiftX, int shiftY, int yDirection){
	
	/* opening input file */
	FILE *inputFile;
	char inputFileName[100];
	sprintf(inputFileName, "%s%s.%d.%d.life", iPathPrefix, type.fileNameRoot, type.phase, type.direction);
	inputFile = fopen( inputFileName, "r");
	if( inputFile == NULL){
		printf("Can't open \"%s\". Try again.\nMake sure that object files must be in %s directory\n", inputFileName, iPathPrefix );
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", inputFileName );
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( of, "a");
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", of);
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", of);
	}

	if(debugFlag != 0)printf("start reading %s\n", inputFileName);
	char tempString[1000];
	tempString[0] = '#';
	while( tempString[0] == '#') fgets( tempString, sizeof(tempString), inputFile);
	
	int eofFlag =0;
	shiftX -= type.xCentre;
	shiftY -= type.yCentre;
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

