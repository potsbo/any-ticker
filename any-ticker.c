#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int outputFileInitialise( char *of);
int debugFlag = 0;
int errorNum = 0;
int dotShift(int base, int shiftNum, int xAreaSize){
	return (base +xAreaSize*shiftNum -5*(shiftNum/2))%xAreaSize;
	/* return base; */
}
const char liveCell = '*';
const char deadCell = '_';
char *iPathPrefix ="./objects/";

int dotMap( char *font, char letter, int size, int dots[64][1000], int xCurrent);


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
	
	int xDefAreaSize = 41;
	int yDefAreaSize = 11;
	double bannerSize = 1.5;

	/* settign objects */
	object eat = {"eater", 4, -11, 0, 0};
	object gun = {"gun", -5, 88, 0, 0};
	object ref = {"reflector", -22, 85, 0, 0,};
	object g01 = {"glider", -23, 68, 0, 1}; object g12 = {"glider", -28, 53, 1, 2};
	object g14 = {"glider", -47, 57, 1, 4}; object g33 = {"glider", -36, 69, 3, 3};
	object g13 = {"glider", -39, 45, 1, 3}; object g21 = {"glider", -35, 79, 2, 1};
	object g23 = {"glider", -50, 57, 2, 3};

	/* setting output file */
	char of[100];
	/* set file name here */	
	strcpy( of, "any-ticker.life");
	printf("input output file name ( default: \"any-ticker.life\"): \n");
	char tempImput[100];
	scanf("%s", tempImput);
	printf("output file name: \"%s\"\n", of);
	outputFileInitialise( of);

	/* set yAreaSize */
	printf("Input y area size ( default: 11): \n");
	int tempNum = 0;
	scanf("%d", &tempNum);
	int yAreaSize = yDefAreaSize;
	if( tempNum > 0) yAreaSize = tempNum;

	/* setting the dot map */
	int dots[64][1000];
	int xCurrent = 0;
	/* read message */
	char message[100] = "g";
	char fontName[100] = "golly";
	int fontSize = yAreaSize;
	printf("Message length: %lu\n", strlen(message));
	for( int i = 0; i < strlen(message); i++){
		xCurrent += dotMap( fontName, message[i], fontSize, dots, xCurrent);
	}
	/* calculating area size; xArea should be 4n + 5(n >= 0) */
	int xAreaSize = 5;
	int refShift= 0;
	while( xAreaSize < xCurrent){
		xAreaSize += 4;
		refShift++;
	}



	/* dot mapping */


	/* for(int y = 0; y < yAreaSize; y++){ */
	/* 	for(int i = 0; i < xAreaSize; i++){ */
	/* 		dots[y][xAreaSize - 1 - i] = string[y] % 2; */
	/* 		string[y] /= 0b10; */
	/* 	} */
	/* } */
	for(int y = 0; y < yAreaSize; y++){

	}


	/* putting guns, reflectors, and gliders */
	int gunNum = fontSize;
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i);
		int xShift = 115 *(i/2);
		int yShift = 18 * (i/2);
		int shiftNum = i;
		int y;
		if( (i%2) == 0){
			y = ( (yAreaSize -1) -i) /2;
		}else{
			y = i /2 +(yAreaSize + 1) /2;
		}
		if( debugFlag != 0)	printf("i: %d, y: %d\n", i, y);
	
		/* gun */
		append( gun, of, -xShift, -yShift, yFlag);
		/* reflector */
		append( ref, of, - xShift +23*refShift, -yShift -23*refShift, yFlag);

		/* gliders */
		for( int i = 0; i < refShift +1; i++){
			if( dots[y][dotShift(i*2,shiftNum,xAreaSize)] == 1)
				append( g01, of, -xShift +23*i, -yShift -23*i, yFlag);
			if( dots[y][dotShift(xAreaSize - 2 - 2*i,shiftNum,xAreaSize)] == 1)
				append( g13, of, -xShift +23*i, -yShift -23*i, yFlag);
		}

		for( int i = 0; i < refShift; i++){
			if( dots[y][dotShift( i*2 + 1,shiftNum,xAreaSize)] == 1)
				append( g21, of, -xShift +23*i, -yShift -23*i, yFlag);
			if( dots[y][dotShift( xAreaSize - 3 - 2*i,shiftNum,xAreaSize)] == 1)
				append( g23, of, -xShift +23*i, -yShift -23*i, yFlag);
		}

		if( dots[y][dotShift( 2 -1 +2*refShift,shiftNum,xAreaSize)] == 1)
			append( g33, of, -xShift + 23*refShift, -yShift -23*refShift, yFlag);
		if( dots[y][dotShift( 3 -1 +2*refShift,shiftNum,xAreaSize)] == 1)
			append( g14, of, -xShift + 23*refShift, -yShift -23*refShift, yFlag);
		if( dots[y][dotShift( 5 -1 +4*refShift,shiftNum,xAreaSize)] == 1)
			append( g12, of, -xShift, - yShift, yFlag);

	}
	printf("%d gun(s) installed\n", gunNum);

	/* putting eaters */
	int distant = 0;
	distant += (int)ceil((xAreaSize *23) *bannerSize /2) *2;
	distant += 115 *  ((gunNum -1) /2) - ((gunNum -1) /2) % 2;
	int eaterNum = gunNum + 2;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag = pow( -1, (i + 2)/2);
		append( eat, of, - distant, - negFlag * 36 * ( (i + 2)/4), yFlag);
	}
	printf("%d eaters installed\n", eaterNum);

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

	printf("\nInitialisation done\n\n");
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

int dotMap( char *font, char letter, int size, int dots[64][1000], int xCurrent){
	char fontFileName[100];
	sprintf( fontFileName, "%s.tfont", font);
	FILE *fp;
	fp = fopen( fontFileName, "r");
	if( fp == NULL){
		printf("Can't open \"%s\". Try again.\n", fontFileName);
		return 1;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", fontFileName);
	}

	char stringTag[100];

	sprintf( stringTag, "#%c%d#\n", letter, size);
	if( debugFlag != 0) printf( "stringTag: %s\n", stringTag);
	char tempString[100] = "##";
	int findFlag = 0;
	// 0: searching // 1: found // -1: not found
	while( findFlag == 0) {
		if( debugFlag != 0)printf("tempString: %s\n", tempString);
		if( fgets( tempString, sizeof(tempString), fp) == NULL){
			findFlag = -1; // not found	
			printf("\"%s\" is not found on %s\n", stringTag, fontFileName);
			printf("%lu\n", strlen(tempString) );
		}else{
			if( strcmp( tempString, stringTag) == 0){
				findFlag = 1; //found
				/* printf("\"%s\" is found on %s\n", stringTag, fontFileName); */
				break;
			}
		}
	}

	fgets( tempString, sizeof(tempString), fp);
	int letterWidth = strlen(tempString) -1;
	printf("%c.letterWidth: %d\n", letter, letterWidth);
	if( findFlag == 1){
		for(int i = 0; i < size; i++){
			int endFlag = 0;
			for( int j = 0; j < letterWidth; j++){
				dots[i][j + xCurrent] = 0;
				printf("%c\n", tempString[j]);
				if(tempString[j] == liveCell){
					dots[i][j + xCurrent] = 1;
					printf("a new live cell prepared to be installed\n");
				}else if(tempString[j] == deadCell){
					dots[i][j + xCurrent] = 0;
					printf("a new dead cell prepared to be installed\n");
				}else{
					printf("no expexted letter in %s:%c\n", fontFileName, tempString[j]);
				}
			}
			fgets( tempString, sizeof(tempString), fp);
		}

	}
	fclose( fp);
	return letterWidth;
}
