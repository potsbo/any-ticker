#include "stdafx.h"
#include "any-ticker.h"
#include "typeset.h"
#include "file_manage.h"
#include <algorithm>

using namespace std;

const char *OBJECT_PATH_PREFIX ="./objects/";
const int S_SIZE = 256;
const int PERIOD = 23;
const int X_MAX = 1024;
const int Y_MAX = 256;

/* interesting parameters */
const int X_DOT_SHIFT = 5; // can't be less than 4
const int Y_UNIT = 18;		// must be 18, otherwise cause bug, which should be fixed

int any_ticker(int argc, char *argv[]){

	/* default values: you don't have to change here */
	const char *outputFileName = "any-ticker.life";
	const char *fontName = "golly";
	const char *message = "golly";
	int xDefAreaSize = 41;	// useless variable
	int yDefAreaSize = 11;	// same as font size
	int extraEaters = 0;	// you can add extra eaters
	double bannerSize = 2;	// banner area is bannersize times longer than message
	int galaxyLess = 2;		// no. of galaxies is less than that of eaters by this
	int yAreaSize = 11;		// default font size
	int promptFlag = 0;

	int tag;
	while( ( tag = getopt( argc, argv, "df:m:o:ps:l:")) != -1){
		switch( tag){
			case 'd':
				debugFlag = 1;
				cout << "debugFlag: " << debugFlag << endl;
				break;
			case 'f':
				fontName = optarg;
				cout << "fontName: " << fontName << endl;
				break;
			case 'm':
				message = optarg;
				cout << "Message: " << message << endl;
				break;
			case 'o':
				outputFileName = optarg;
				cout << "Output file name: " << outputFileName << endl;
				break;
			case 'p':
				promptFlag = 1;
				cout << "Prompt feature needs working" << endl;
				break;
			case 's':
				if( (yAreaSize = atoi(optarg)) != 0)
					cout << "Input an integer for font size" << endl;
				break;
			case 'l':
				sscanf( optarg, "%lf", &bannerSize);
				cout << "bannerSize: " << bannerSize << endl;
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
	cout << endl << "Start reading font file to set dot map" << endl;
	cout << "Message lenght: " << strlen(message) << endl;
	int dots[1024][256]; 	// each dot
	int xLeastAreaSize = 0;
	for( int i = 0; i < strlen(message); i++)
		xLeastAreaSize += typeSetMapping( fontName, message[i],
				yAreaSize, dots, xLeastAreaSize);
	/* checking space between the last letter and the first */
	if( letterSpaceCheck( dots, 0, xLeastAreaSize -1, yAreaSize) > 0) xLeastAreaSize++; 
	cout << "Dot map created" << endl;


	/* calculating area size; xArea should be 4n + 5 (n >= 0) */
	cout << endl << "Start calculating x area size" << endl;
	cout << "xLeastAreaSize: " << xLeastAreaSize << endl;
	int xAreaSize = 5; // because the minimun gun has 5 positions to have a glider 
	/* set xAreaSize to proper value */
	while( xAreaSize < xLeastAreaSize) xAreaSize += 4;
	cout << "Calculated xAreaSize: " << xAreaSize << endl;

	/* set dots in blank space to zero */
	for(int y = 0; y < yAreaSize; y++)
		for(int x = 0; x < xAreaSize - xLeastAreaSize; x++)
			dots[xLeastAreaSize +x][y] = 0;


	/* output file initialisation */
	outputFileInitialise( outputFileName, "#Life 1.06\n");

	/* putting guns, reflectors, and gliders */
	cout << endl << "Start installing objects" << endl;

	/* installing ships( temporary glider eater) */
	int gunNum = yAreaSize;
	int delMax = 0;		// max number of useless dots(gliders) of each gun
	int adjustFlag = 1; /* for debug */
	if( adjustFlag == 1){
		int shipNum = 0;	// one ship can delete two dots(gliders)
		for( int i = 0; i < gunNum; i++){
			int xShift = X_DOT_SHIFT *PERIOD *(i/2);// guns and reflectors shifted by this
			int yShift = Y_UNIT *(i/2);
			int yFlag = pow(-1, i); // make object upside down
			int uselessDots = 0;	// the number of useless dots
			int y = ( gunNum -yFlag *i +i%2)/2;

			/* counting useless dots for current gun */
			int cycle = ( X_DOT_SHIFT*(i/2) + xAreaSize -1) /xAreaSize; 
			// the number of useless cycle
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
				installObject( blk, outputFileName, -xShift -shpNum*4,
						-yShift -shpNum*4, yFlag);
		}
		if( debugFlag != 0) cout << "delMax: " << delMax << endl;
		cout << "ship(s) and block(s) installed to delete up to";
		cout << delMax <<" dot(s) per one y line" << endl;
	}

	/* calculating where to put gliders and reflectors */
	int delShift = 0;
	while( delShift *PERIOD + 41 < (delMax+1)/2 *4) delShift++;
	cout << "delShift: " << delShift << endl;

	/* guns and reflectors */
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i);						// make object upside down
		int xShift = X_DOT_SHIFT *PERIOD *(i/2);	// guns and reflectors shifted by this
		int yShift = Y_UNIT *(i/2);
		int refShift = (xAreaSize -5) /4;			// reflector shift depens on xAreaSize

		/* guns */
		installObject( dup, outputFileName, -xShift -delShift *PERIOD,
				-yShift -delShift *PERIOD, yFlag);
		installObject( lws, outputFileName, -xShift, -yShift, yFlag);

		/* reflectors */
		installObject( ref, outputFileName, - xShift +PERIOD*refShift -delShift *PERIOD,
				-yShift -PERIOD*refShift -delShift *PERIOD, yFlag);
	}
	cout << gunNum;
	cout << " set(s) of duplicator(s), lwssmaker(s), and reflector(s) installed" << endl;

	/* installing gliders */
	installGliders( glider, dots, xAreaSize, delShift, gunNum, outputFileName);


	/* installing eaters */
	int distance = 6;// distance between eaters and guns
	while( distance < xAreaSize *PERIOD *bannerSize) distance += 4;
	cout << "distance: " << distance << endl;
	distance += X_DOT_SHIFT *PERIOD * ((gunNum -1) /2);
	distance -= ((gunNum -1) /2) % 2; /* adjusting parity */
	//eaters shifted because of the number of guns
	int eaterNum = gunNum + abs(extraEaters);
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag = pow( -1, (i + 2)/2);
		installObject( eat, outputFileName, - distance,
				-negFlag * 2*Y_UNIT * ( (i + 2)/4), yFlag);
	}
	cout << eaterNum << " eaters installed" << endl;


	/* installing galaxies (both right and left of eaters) */	
	int galaxyNum = max( gunNum -galaxyLess, 1);
	for( int i = -(galaxyNum + 1)/2 ; i < galaxyNum/2; i++){
		/* each row */
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
		/* ( firstLive == xAreaSize) means there is no live cell in that row */

		/* installing the appropriate phase of galaxy */
		if( firstLive != xAreaSize){ 
			int genToGlx = distance *2;
			genToGlx += 229 +123; /* <Generations to first lwss> + <adjust num> */
			genToGlx += firstLive *PERIOD *2;
				/* actually useless because (firstLive *PERIOD *2) %8 = 0 */
			genToGlx += firstLive *4;
			genToGlx += delShift *4;
			if( ( (y + ( gunNum+1)/2)%2) %2 != 0)
				/* want to make this simple */
				installObject( galaxy[(genToGlx)%8], outputFileName,
						-distance+24, 18*i, 1);
			else
				installObject( galaxy[(genToGlx+6)%8], outputFileName,
						-distance+23, 18*i, 1);
		}
		/* end of a row */
	}


	cout << endl << "End combining with " << errorNum << " error(s)" << endl;
	return 0;
}

int installObject( object type, const char *of, int shiftX, int shiftY, int yDirection){
	
	/* generating file name */
	char inputFileName[S_SIZE];
	sprintf(inputFileName, "%s%s.%d.%d.life", OBJECT_PATH_PREFIX, type.fileNameRoot, type.phase, type.direction);
	
	/* opening input file */
	FILE *inputFile;
	inputFile = fopen( inputFileName, "r");

	/* checking the file */
	if( inputFile == NULL){
		cout << "Can't open " << inputFileName;
		cout << "Try again." << endl;
		cout << "Make sure that object fils must be in " << OBJECT_PATH_PREFIX << endl;
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) cout << inputFileName << "is successfully opened" << endl;
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( of, "a");

	/* checking the file */
	if( outputFile == NULL){
		cout << "Can't open " << of << endl;
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) cout << of << "is successfully opened" << endl;
	}

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
		}
	}
	fclose( inputFile);
	fclose( outputFile);

	return 0;
}

int dotShift(int base, int shiftNum, int xAreaSize){
	return (base +xAreaSize*shiftNum -X_DOT_SHIFT*(shiftNum/2))%xAreaSize;
}

int installGliders( object *glider, int dots[][256], int xAreaSize, int delShift, int gunNum, const char *of){

	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i); // make object upside down
		int xShift = ( X_DOT_SHIFT *(i/2) + delShift)*PERIOD; // guns, reflectors, and gliders shifted by this value
		int yShift = Y_UNIT *(i/2) + delShift *PERIOD;
		int shiftNum = i;
		int refShift = (xAreaSize -5) /4; // reflector shifted depending on xAreaSize
		int y = ( gunNum -yFlag *i +i%2)/2;

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
