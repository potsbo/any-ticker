#include "stdafx.h"
#include "any-ticker.h"
#include "typeset_mapping.h"

int main(int argc, char *argv[]){

	/* char *outputFileName = NULL; */
	char *outputFileName = "any-ticker.life";
	char *fontName = "golly";
	char *message = "golly";
	int yAreaSize = 11;
	int promptFlag = 0;

	int tag;
	while( ( tag = getopt( argc, argv, "df:m:o:ps:")) != -1){
		switch( tag){
			case 'd':
				debugFlag = 1;
				printf("Debug Flag %d\n", debugFlag);
				break;
			case 'f':
				fontName = optarg;
				break;
			case 'm':
				message = optarg;
				break;
			case 'o':
				outputFileName = optarg;
				break;
			case 'p':
				promptFlag = 1;
				printf("Prompt feature needs working\n");
				break;
			case 's':
				if( (yAreaSize = atoi(optarg)) != 0)
					printf("Input Integer for font size\n");
				break;
			case '?':
				if( optopt == 'o')
					printf( "Option -%c requires an argument.\n", optopt);
				else if( isprint( optopt))
					printf( "Unknown option `-%c'.\n", optopt);
				else
					printf ( "Unknown option character `\\x%x'.\n", optopt);
				return 1;
		}
	}

	/* default values: you don't have to change here */
	int xDefAreaSize = 41;	// useless variable
	int yDefAreaSize = 11;	// same as font size
	int extraEaters = 0;	// you can add extra eaters
	double bannerSize = 2;	// banner area is bannersize times longer than message
	int galaxyLess = 2;		// the number of galaxies is less than that of eaters by this variable

	/* setting objects */
	/* {type, xCentre, yCentre, phase, direction} */
	object eat = {"eater", 4, -11, 0, 0};
	object dup = {"duplicator", -5, 88, 0, 0};
	object lws = {"lwssmaker", -83, 31, 0, 0};
	object ref = {"reflector", -22, 85, 0, 0,};
	object shp = {"ship", -77, 10, 0, 0,};
	object blk = {"block", -78, 9, 0, 0,};
	object glider[7] = {
		{"glider", -23, 68, 0, 1}, {"glider", -28, 53, 1, 2},
		{"glider", -47, 57, 1, 4}, {"glider", -36, 69, 3, 3},
		{"glider", -39, 45, 1, 3}, {"glider", -35, 79, 2, 1},
		{"glider", -50, 57, 2, 3}
	};
	object galaxy[8] = {
		{"galaxy", 17, -6, 0, 0}, {"galaxy", 17, -6, 1, 0},
		{"galaxy", 17, -6, 2, 0}, {"galaxy", 18, -5, 3, 0},
		{"galaxy", 19, -4, 4, 0}, {"galaxy", 19, -4, 5, 0},
		{"galaxy", 18, -5, 6, 0}, {"galaxy", 18, -5, 7, 0},
	};

	/* setting the dot map */
	/* reading font file and ticker message */
	printf("\nStart reading font file to set dot map\n");
	printf("Message length: %lu\n", strlen(message));
	int dots[X_MAX][Y_MAX]; 	// each dot
	int xLeastAreaSize = 0;
	for( int i = 0; i < strlen(message); i++)
		xLeastAreaSize += typeSetMapping( fontName, message[i], yAreaSize, dots, xLeastAreaSize);
	/* checking space between the last letter and the first */
	if( letterSpaceCheck( dots, 0, xLeastAreaSize -1, yAreaSize) > 0) xLeastAreaSize++; 
	printf("Dot map created\n");


	/* calculating area size; xArea should be 4n + 5 (n >= 0) */
	printf("\nStart calculating x area size\n");
	printf("x least area size: %d\n", xLeastAreaSize);
	int xAreaSize = 5; // because the minimun gun has 5 positions to have a glider 
	/* set xAreaSize to proper value */
	while( xAreaSize < xLeastAreaSize) xAreaSize += 4;
	printf("Calculated x area size: %d\n", xAreaSize);

	/* set dots in blank space to zero */
	for(int y = 0; y < yAreaSize; y++)
		for(int x = 0; x < xAreaSize - xLeastAreaSize; x++)
			dots[xLeastAreaSize +x][y] = 0;


	/* output file initialisation */
	outputFileInitialise( outputFileName);

	/* putting guns, reflectors, and gliders */
	printf("\nStart installing objects\n");

	/* installing ships( temporary glider eater) */
	int gunNum = yAreaSize;
	int delMax = 0;		// max number of useless dots(gliders) of each gun
	int shipNum = 0;	// one ship can delete two dots(gliders)
	for( int i = 0; i < gunNum; i++){
		int xShift = X_DOT_SHIFT *PERIOD *(i/2);// guns and reflectors shifted by this value
		int yShift = Y_UNIT *(i/2);
		int yFlag = pow(-1, i); // make object upside down
		int uselessDots = 0;	// the number of useless dots
		int y = ( gunNum -yFlag *i +i%2)/2;

		/* counting useless dots for current gun */
		int cycle = ( X_DOT_SHIFT*(i/2) + xAreaSize -1) /xAreaSize; // the number of useless cycle
		for( int x =dotShift(0,i,xAreaSize ); x < xAreaSize *cycle; x++)
			if( dots[x %xAreaSize][y] == 1)
				uselessDots++;

		/* updating record */
		delMax = max( uselessDots, delMax);
		
		int shpNum = uselessDots /2; 	// one ship deletes 2 gliders
		int blkNum = uselessDots %2;	// one block deletes 1 glider
		for( int i = 0; i < shpNum; i++)
			installObject( shp, outputFileName, -xShift -i *4, -yShift -i*4, yFlag);
		if( blkNum == 1)
			installObject( blk, outputFileName, -xShift -shpNum*4, -yShift -shpNum*4, yFlag);
	}
	if( debugFlag != 0) printf("delMax: %d\n", delMax);
	printf("ship(s) and block(s) installed to delete up to %d dot(s) per one y line\n", delMax);

	/* calculating where to put gliders and reflectors */
	int delShift = 0;
	while( delShift *PERIOD + 41 < (delMax+1)/2 *4) delShift++;

	/* guns and reflectors */
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i);						// make object upside down
		int xShift = X_DOT_SHIFT *PERIOD *(i/2);	// guns and reflectors shifted by this value
		int yShift = Y_UNIT *(i/2);
		int refShift = (xAreaSize -5) /4;			// reflector shifted depending on xAreaSize
	
		/* guns */
		installObject( dup, outputFileName, -xShift -delShift *PERIOD, -yShift -delShift *PERIOD, yFlag);
		installObject( lws, outputFileName, -xShift, -yShift, yFlag);

		/* reflectors */
		installObject( ref, outputFileName, - xShift +PERIOD*refShift -delShift *PERIOD, -yShift -PERIOD*refShift -delShift *PERIOD, yFlag);
	}
	printf("%d set(s) of duplicator(s), lwssmaker(s), and reflector(s) installed\n", gunNum);
	

	/* installing gliders */
	installGliders( glider, dots, xAreaSize, delShift, gunNum, outputFileName);


	/* installing eaters */
	int distance = 6;// distance between eaters and guns
	while( distance < xAreaSize *PERIOD *bannerSize) distance += PERIOD*4;
	/* distance += (int)ceil((xAreaSize *PERIOD) *bannerSize /2) *2; */
	distance += X_DOT_SHIFT *PERIOD * ((gunNum -1) /2) - ((gunNum -1) /2) % 2; //eaters shifted because of the number of guns
	int eaterNum = gunNum + abs(extraEaters);
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag = pow( -1, (i + 2)/2);
		installObject( eat, outputFileName, - distance, - negFlag * 2*Y_UNIT * ( (i + 2)/4), yFlag);
	}
	printf("%d eaters installed\n", eaterNum);


	/* installing galaxies */	
	int galaxyNum = max( gunNum -galaxyLess, 2);
	for( int i = -(galaxyNum + 1)/2 ; i < galaxyNum/2; i++){
		int y = i + ( gunNum +1)/2;
		while(y < 0)y += 8;

		/* galaxies on the left of eaters */
		installObject( galaxy[y%8], outputFileName, -distance, 18*i, 1);

		/* calculating which galaxy to have to make it a temporary eater */
		int firstLive = xAreaSize; 
		for(int x = 0; x < xAreaSize; x++){
			if(dots[x][y] == 1){
				firstLive = x;
				break;
			}
		}

		/* installing the appropriate phase of galaxy */
		if( firstLive != xAreaSize){
			if( y %2 == 0) installObject( galaxy[(firstLive*2+6)%8], outputFileName, -distance+23, 18*i, 1);
			else installObject( galaxy[(firstLive*2)%8], outputFileName, -distance+24, 18*i, 1);
		}
	}


	printf("\nEnd combining with %d error(s)\n", errorNum);
}

int outputFileInitialise( char *of){

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
	fprintf( outputFile, "#Life 1.06\n");

	/* close */
	fclose( outputFile);

	printf("\nInitialisation done\n");
	return 0;
}

int installObject( object type, char *of, int shiftX, int shiftY, int yDirection){
	
	/* generating file name */
	char inputFileName[S_SIZE];
	sprintf(inputFileName, "%s%s.%d.%d.life", OBJECT_PATH_PREFIX, type.fileNameRoot, type.phase, type.direction);
	
	/* opening input file */
	FILE *inputFile;
	inputFile = fopen( inputFileName, "r");

	/* checking the file */
	if( inputFile == NULL){
		printf("Can't open \"%s\". Try again.\nMake sure that object files must be in %s directory\n", inputFileName, OBJECT_PATH_PREFIX );
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", inputFileName );
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( of, "a");

	/* checking the file */
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", of);
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", of);
	}

	if(debugFlag != 0)printf("start reading %s\n", inputFileName);
	char tempString[X_MAX];
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

int typeSet( char *font, char letter, int size, int dots[X_MAX][Y_MAX], const int x){

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
	char tempString[S_SIZE] = "##";
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

int dotShift(int base, int shiftNum, int xAreaSize){
	return (base +xAreaSize*shiftNum -X_DOT_SHIFT*(shiftNum/2))%xAreaSize;
}

int letterSpaceCheck(int dots[X_MAX][Y_MAX], int x, int xTarget, int size){
	int letterSpaceFlag = 0;
	letterSpaceFlag += dots[x][0] *( dots[xTarget][0] +dots[xTarget][0 +1] );
	letterSpaceFlag += dots[x][size-1] *( dots[xTarget][size-1] +dots[xTarget][size -1]);
	for( int i = 0+1; i < size-1; i++){
		letterSpaceFlag += dots[x][i] *( dots[xTarget][i] +dots[xTarget][i -1] +dots[xTarget][i +1]);
	}
	if( letterSpaceFlag > 0) printf("Letter space inserted\n");
	return letterSpaceFlag;
}


int installGliders( object *glider, int dots[X_MAX][Y_MAX], int xAreaSize, int delShift, int gunNum, char of[S_SIZE]){

	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i); // make object upside down
		int xShift = ( X_DOT_SHIFT *(i/2) + delShift)*PERIOD; // guns, reflectors, and gliders shifted by this value
		int yShift = Y_UNIT *(i/2) + delShift *PERIOD;
		int shiftNum = i;
		int refShift = (xAreaSize -5) /4; // reflector shifted depending on xAreaSize
		int y = ( gunNum -yFlag *i +i%2)/2;

		if( debugFlag != 0)	printf("i: %d, y: %d\n", i, y);
	
		/* gliders */
		for( int i = 0; i < refShift +1; i++){
			if( dots[dotShift(i*2,shiftNum,xAreaSize)][y] == 1)
				installObject( glider[0], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
			if( dots[dotShift(xAreaSize - 2 - 2*i,shiftNum,xAreaSize)][y] == 1)
				installObject( glider[4], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
		}

		for( int i = 0; i < refShift; i++){
			if( dots[dotShift( i*2 + 1,shiftNum,xAreaSize)][y] == 1)
				installObject( glider[5], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
			if( dots[dotShift( xAreaSize - 3 - 2*i,shiftNum,xAreaSize)][y] == 1)
				installObject( glider[6], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
		}

		if( dots[dotShift( 2 -1 +2*refShift,shiftNum,xAreaSize)][y] == 1)
			installObject( glider[3], of, -xShift + PERIOD*refShift, -yShift -PERIOD*refShift, yFlag);
		if( dots[dotShift( 3 -1 +2*refShift,shiftNum,xAreaSize)][y] == 1)
			installObject( glider[2], of, -xShift + PERIOD*refShift, -yShift -PERIOD*refShift, yFlag);
		if( dots[dotShift( 5 -1 +4*refShift,shiftNum,xAreaSize)][y] == 1)
			installObject( glider[1], of, -xShift, - yShift, yFlag);

	}
	return 0;
}
