#include <stdio.h>
#include <string.h>
#include <math.h>

int append( char *inputFileName, char *outputFileName, int shiftX, int shiftY, int yDirection);
int outputFileInitialise( char *outputFileName);
int debugFlag = 0;

int main(int argc, char *argv[]){
	/* int yEatersCentre = 79; */
	/* int xEatersCentre = 18; */
	/* int yGunCentre = 292; */
	/* int xGunCentre = -5; //Can't be closer than this */
	
	char eaterFileName[100];
	/* set file name here */
	strcpy( eaterFileName, "eater.life");
	int yEaterCentre = -11;
	int xEaterCentre = 4;

	char gunPart1FileName[100];
	/* set file name here */
	strcpy( gunPart1FileName, "ticker.gun.part1.life");
	int xGunPart1Centre = -5; //Can't be closer than this
	int yGunPart1Centre = 88;

	char gunPart2FileName[100];
	/* set file name here */
	strcpy( gunPart2FileName, "ticker.gun.part2.life");
	int xGunPart2Centre = -22; //Can't be closer than this
	int yGunPart2Centre = 85;
	
	char gliderPhase1FileName[100];
	/* set file name here */
	strcpy( gliderPhase1FileName, "glider.phase1.life");
	int xGlider1Centre = -23;
	int yGlider1Centre = 68;
	
	char gliderPhase1DownFileName[100];
	/* set file name here */
	strcpy( gliderPhase1DownFileName, "glider.phase1.down.life");
	int xGlider1DownCentre = -28;
	int yGlider1DownCentre = 53;
	
	char gliderPhase1NwFileName[100];
	/* set file name here */
	strcpy( gliderPhase1NwFileName, "glider.phase1.nw.life");
	int xGlider1NwCentre = -47;
	int yGlider1NwCentre = 57;

	char gliderPhase3UpFileName[100];
	/* set file name here */
	strcpy( gliderPhase3UpFileName, "glider.phase3.up.life");
	int xGlider3UpCentre = -36;
	int yGlider3UpCentre = 69;
	
	char gliderPhase3NeFileName[100];
	/* set file name here */
	strcpy( gliderPhase3NeFileName, "glider.phase3.ne.life");
	int xGlider3NeCentre = -39;
	int yGlider3NeCentre = 45;

	char gliderPhase2SwFileName[100];
	/* set file name here */
	strcpy( gliderPhase2SwFileName, "glider.phase2.sw.life");
	int xGlider2SwCentre = -35;
	int yGlider2SwCentre = 79;

	char gliderPhase2NeFileName[100];
	/* set file name here */
	strcpy( gliderPhase2NeFileName, "glider.phase2.ne.life");
	int xGlider2NeCentre = -50;
	int yGlider2NeCentre = 57;

	char outputFileName[100];
	/* set file name here */	
	strcpy( outputFileName, "any-ticker.life");
	outputFileInitialise( outputFileName);
	int xLeastAreaSize = 41;
	int xAreaSize = 5;
	int gunPart2Shift = 0;
	while( xAreaSize < xLeastAreaSize){
		xAreaSize += 4;
		gunPart2Shift++;
	}

	/* putting guns and gliders */
	int gunNum = 1;
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1,i );
		int xShift = 115 *(i/2);
		int yShift = 18 * (i/2);
		/* guns */
		append( gunPart1FileName, outputFileName, -xGunPart1Centre - xShift, -yGunPart1Centre - yShift, yFlag);
		append( gunPart2FileName, outputFileName, -xGunPart2Centre - xShift + 23*gunPart2Shift, -yGunPart2Centre - yShift - 23*gunPart2Shift, yFlag);

		/* gliders */
		for( int i = 0; i < gunPart2Shift + 1; i++){
			append( gliderPhase1FileName, outputFileName, -xGlider1Centre - xShift + 23*i, -yGlider1Centre - yShift -23*i, yFlag);
			append( gliderPhase3NeFileName, outputFileName, -xGlider3NeCentre - xShift +23*i, -yGlider3NeCentre - yShift -23*i, yFlag);
		}
		for( int i = 0; i < gunPart2Shift; i++){
			append( gliderPhase2SwFileName, outputFileName, -xGlider2SwCentre - xShift +23*i , -yGlider2SwCentre - yShift -23*i, yFlag);
			append( gliderPhase2NeFileName, outputFileName, -xGlider2NeCentre - xShift +23*i , -yGlider2NeCentre - yShift -23*i, yFlag);
		}
		append( gliderPhase1DownFileName, outputFileName, -xGlider1DownCentre - xShift , -yGlider1DownCentre - yShift, yFlag);

		append( gliderPhase1NwFileName, outputFileName, -xGlider1NwCentre - xShift + 23*gunPart2Shift, -yGlider1NwCentre - yShift -23*gunPart2Shift, yFlag);
		append( gliderPhase3UpFileName, outputFileName, -xGlider3UpCentre - xShift + 23*gunPart2Shift, -yGlider3UpCentre - yShift -23*gunPart2Shift, yFlag);
	}
	printf("%d gun(s) put\n", gunNum);

	/* putting eaters */
	int distant = 0;
	double bannerSize = 1.5;
	distant += (int)ceil( (xAreaSize * 23) * bannerSize / 2) *2 ;
	distant += 115 *  ( (gunNum - 1) / 2) - ((gunNum -1) / 2) % 2;
	int eaterNum = gunNum + 2;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag = pow( -1, (i + 2)/2);
		append( eaterFileName, outputFileName, -xEaterCentre - distant, -yEaterCentre - negFlag * 36 * ( (i + 2)/4), yFlag);
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

	printf("Initialisation done\n\n");
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
