#include "stdafx.h"
#include "typeset.h"
const char *FONT_PATH_PREFIX="./fonts/";
const char LIVE_CELL = '*';
const char DEAD_CELL = '_';

int typeSetMapping( char *font, char letter, int size, int dots[1024][256], const int x){

	/* skipping line feed */
	if( letter == 10){
		printf("Line feed skipped\n");
		return 0;
	}

	/* generating font file name */
	char fontFileName[S_SIZE];
	const char *lastSix = &font[strlen(font)-6];
	if( strcmp( lastSix, ".tfont") !=0 ) 
		sprintf( fontFileName, "%s%s.tfont",FONT_PATH_PREFIX, font);

	/* opening font file */
	FILE *fp;
	fp = fopen( fontFileName, "r");
	if( fp == NULL){
		printf("Can't open \"%s\". Try again.\nMake sure that font files must be in %s directory\n", fontFileName, FONT_PATH_PREFIX );
		errorNum++;
		return 0;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", fontFileName);
	}

	/* searching letter data on the file */
	char stringTag[S_SIZE];
	sprintf( stringTag, "#%c%d#\n", letter, size);
	if( debugFlag != 0) printf( "stringTag: %s\n", stringTag);
	char tempString[S_SIZE];
	int findFlag = 0;
	// 0: searching // 1: found // -1: not found
	while( findFlag == 0){
		if( debugFlag != 0) printf("tempString: %s\n", tempString);

		if( fgets( tempString, sizeof(tempString), fp) == NULL){
			char *shortString[S_SIZE];
			printf("\"%c\" with size %d was not found on %s\n", letter, size, fontFileName);
			errorNum++;
			return 0;
		}else 
			if( strcmp( tempString, stringTag) == 0) findFlag = 1; //found
	}

	/* settig letter width */
	while( tempString[0] == '#' || tempString[0] == '\n')
		fgets( tempString, sizeof(tempString), fp);
	int letterWidth = strlen(tempString) -1;
	printf("%c.letterWidth: %d\n", letter, letterWidth);

	/* setting dot map */
	for(int y = 0; y < size; y++){
		for( int j = 0; j < letterWidth; j++){

			if( debugFlag != 0)printf("%c:", tempString[j]);

			/* initialisation */
			dots[j + x][y] = 0;

			/* mapping */
			if(tempString[j] == LIVE_CELL){
				dots[j + x][y] = 1;
				if( debugFlag != 0)printf("a new live cell prepared to be installed\n");
			}else if(tempString[j] == DEAD_CELL){
				dots[j + x][y] = 0;
				if( debugFlag != 0)printf("a new dead cell prepared to be installed\n");
			}else{
				printf("no expexted letter in %s:%c (%d)\n", fontFileName, tempString[j], tempString[j]);
				errorNum++;
			}
		}
		fgets( tempString, sizeof(tempString), fp);
	}

	/* checking space between previous one */
	int letterSpaceFlag = 0;
	if( x != 0) letterSpaceFlag = letterSpaceCheck( dots, x, x -1, size);

	/* shifting a letter if too close to previous one */
	if(letterSpaceFlag > 0){
		for(int y = 0; y < size; y++){
			for(int j = 0; j < letterWidth; j++)
				dots[x + letterWidth -j][y] = dots[x +letterWidth -j -1][y];
			dots[x][y] = 0;
		}
		letterWidth++;
	}


	fclose( fp);
	return letterWidth;
}

int letterSpaceCheck(int dots[1024][256], int x, int xTarget, int size){
	int letterSpaceFlag = 0;
	letterSpaceFlag += dots[x][0] *( dots[xTarget][0] +dots[xTarget][0 +1] );
	letterSpaceFlag += dots[x][size-1] *( dots[xTarget][size-1] +dots[xTarget][size -1]);
	for( int i = 0+1; i < size-1; i++){
		letterSpaceFlag += dots[x][i] *( dots[xTarget][i] +dots[xTarget][i -1] +dots[xTarget][i +1]);
	}
	if( letterSpaceFlag > 0) printf("Letter space inserted\n");
	return letterSpaceFlag;
}
