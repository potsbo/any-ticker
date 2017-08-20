#include "stdafx.h"
#include "any-ticker.h"
#include "typeset.h"
#include "file_manage.h"
#include "life_object.h"
#include <algorithm>
#include <string>

using namespace std;

const int S_SIZE = 256;
const int PERIOD = 23;

/* interesting parameters */
const int X_DOT_SHIFT = 5; // can't be less than 4
const int Y_UNIT = 18;		// must be 18, otherwise cause bug, which should be fixed

int LifeObject::xShift, LifeObject::yShift;

int installGliders( LifeObject *glider, int dots[][256], int xAreaSize, int delShift, int yAreaSize, const char *of);

int any_ticker(int argc, char *argv[]){

	/* default values: you don't have to change here */
	string outputFileName = "any-ticker.life";
	string fontName = "golly";
	string message = "golly";
	int xDefAreaSize = 41;	// useless variable
	int yDefAreaSize = 11;	// same as font size
	int extraEaters = 0;	// you can add extra eaters
	double bannerSize = 2.425;	// banner area is bannersize times longer than message
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
	LifeObject eat, dup, lws, ref, shp, blk, glider[7], galaxy[8];
	eat.Set(       "eater",   4,-11, 0, 0);
	dup.Set(  "duplicator",  -5, 88, 0, 0);
	lws.Set(   "lwssmaker", -83, 31, 0, 0);
	ref.Set(   "reflector", -22, 85, 0, 0);
	shp.Set(        "ship", -77, 10, 0, 0);
	blk.Set(       "block", -78,  9, 0, 0);
	glider[0].Set("glider", -23, 68, 0, 1);
	glider[1].Set("glider", -28, 53, 1, 2);
	glider[2].Set("glider", -47, 57, 1, 4);
	glider[3].Set("glider", -36, 69, 3, 3);
	glider[4].Set("glider", -39, 45, 1, 3);
	glider[5].Set("glider", -35, 79, 2, 1);
	glider[6].Set("glider", -50, 57, 2, 3);
	galaxy[0].Set("galaxy",  17, -6, 0, 0);
	galaxy[1].Set("galaxy",  17, -6, 1, 0);
	galaxy[2].Set("galaxy",  17, -6, 2, 0);
	galaxy[3].Set("galaxy",  18, -5, 3, 0);
	galaxy[4].Set("galaxy",  19, -4, 4, 0);
	galaxy[5].Set("galaxy",  19, -4, 5, 0);
	galaxy[6].Set("galaxy",  18, -5, 6, 0);
	galaxy[7].Set("galaxy",  18, -5, 7, 0);
    
	/* setting the dot map */
	/* reading font file and ticker message */
	cout << endl << "Start reading font file to set dot map" << endl;
	cout << "Message length: " << message.length() << endl;
	int dots[1024][256]; 	// each dot
	int xLeastAreaSize = 0;
	for( int i = 0; i < message.length(); i++)
		xLeastAreaSize += typeSetMapping( fontName.c_str(), message[i],
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
	outputFileInitialise( outputFileName.c_str(), "#Life 1.06\n");


	/* putting guns, reflectors, and gliders */
	cout << endl << "Start installing objects" << endl;

	/* calculating distance */
	int distance = 4;// distance between eaters and guns
	while( distance < xAreaSize *PERIOD *bannerSize) distance += 4;
	cout << "distance: " << distance << endl;
	distance += X_DOT_SHIFT *PERIOD * ((yAreaSize -1) /2);
	distance -= ((yAreaSize -1) /2) % 2; /* adjusting parity */

	/* installing ships( temporary glider eater) */
	int gunNum = yAreaSize;
	int delMax = 0;		// max number of useless dots(gliders) of each gun
	int adjustFlag = 1; /* for debug */
	if( adjustFlag == 1){
		int shipNum = 0;	// one ship can delete two dots(gliders)
		for( int i = 0; i < gunNum; i++){
			/* each row */
			LifeObject::xShift = X_DOT_SHIFT *PERIOD *(i/2);
			// guns and reflectors shifted by this
			LifeObject::yShift = Y_UNIT *(i/2);
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
				shp.install( outputFileName.c_str(), -i*4, -i*4, yFlag);
			if( blkNum == 1)
				blk.install( outputFileName.c_str(), -shpNum*4, -shpNum*4, yFlag);
			/* end of a row */
		}
		if( debugFlag != 0) cout << "delMax: " << delMax << endl;
		cout << "ship(s) and block(s) installed to delete up to ";
		cout << delMax <<" dot(s) per one y line" << endl;
	}

	/* calculating where to put gliders and reflectors */
	int delShift = 0;
	while( delShift *PERIOD + 41 < (delMax+1)/2 *4) delShift++;
	cout << "delShift: " << delShift << endl;

	/* guns and reflectors */
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i);						// make object upside down
		LifeObject::xShift = X_DOT_SHIFT *PERIOD *(i/2);	// guns and reflectors shifted by this
		LifeObject::yShift = Y_UNIT *(i/2);
		int refShift = (xAreaSize -5) /4;			// reflector shift depens on xAreaSize

		/* guns */
		dup.install( outputFileName.c_str(), -delShift *PERIOD, -delShift *PERIOD, yFlag);
		lws.install( outputFileName.c_str(), 0, 0, yFlag);

		/* reflectors */
		ref.install( outputFileName.c_str(), +PERIOD*refShift -delShift *PERIOD,
				-PERIOD*refShift -delShift *PERIOD, yFlag);
	}
	cout << gunNum;
	cout << " set(s) of duplicator(s), lwssmaker(s), and reflector(s) installed" << endl;

	/* installing gliders */
	installGliders( glider, dots, xAreaSize, delShift, gunNum, outputFileName.c_str());

	/* installing eaters */
	//eaters shifted because of the number of guns
	int eaterNum = gunNum + abs(extraEaters);
  LifeObject::xShift = LifeObject::yShift = 0;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, (i+3)/2);
		int negFlag = pow( -1, (i+2)/2);
		eat.install( outputFileName.c_str(), -distance,
				-negFlag * 2*Y_UNIT * ( (i + 2)/4), yFlag);
	}
	cout << eaterNum << " eaters installed" << endl;


	/* installing galaxies (both right and left of eaters) */	
	int galaxyNum = max( gunNum -galaxyLess, 1);
	for( int i = -(galaxyNum + 1)/2 ; i < galaxyNum/2; i++){
		/* each row */
		int y = i + ( gunNum +1)/2;
		while(y < 0) y +=8;

		/* galaxies on the left of eaters */
		galaxy[y%8].install( outputFileName.c_str(), -distance, 18*i, 1);

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
				galaxy[(genToGlx)%8].install( outputFileName.c_str(),
						-distance+24, 18*i, 1);
			else
				galaxy[(genToGlx+6)%8].install( outputFileName.c_str(),
						-distance+23, 18*i, 1);
		}
		/* end of a row */
	}


	cout << endl << "End combining with " << errorNum << " error(s)" << endl;
	return 0;
}



int dotShift(int base, int shiftNum, int xAreaSize){
	return (base +xAreaSize*shiftNum -X_DOT_SHIFT*(shiftNum/2))%xAreaSize;
}

int installGliders( LifeObject *glider, int dots[][256], int xAreaSize, int delShift, int gunNum, const char *of){

	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i); // make object upside down
		LifeObject::xShift = ( X_DOT_SHIFT *(i/2) + delShift)*PERIOD;// gliders shifted by this
		LifeObject::yShift = Y_UNIT *(i/2) + delShift *PERIOD;
		int shiftNum = i;
		int refShift = (xAreaSize -5) /4; // reflector shifted depending on xAreaSize
		int y = ( gunNum -yFlag *i +i%2)/2;

		/* gliders */
		for( int i = 0; i < refShift +1; i++){
			if( dots[dotShift(i*2,shiftNum,xAreaSize)][y] == 1)
				glider[0].install( of, +PERIOD*i, -PERIOD*i, yFlag);
			if( dots[dotShift(xAreaSize - 2 - 2*i,shiftNum,xAreaSize)][y] == 1)
				glider[4].install( of, +PERIOD*i, -PERIOD*i, yFlag);
		}

		for( int i = 0; i < refShift; i++){
			if( dots[dotShift( i*2 + 1,shiftNum,xAreaSize)][y] == 1)
				glider[5].install( of, +PERIOD*i, -PERIOD*i, yFlag);
			if( dots[dotShift( xAreaSize - 3 - 2*i,shiftNum,xAreaSize)][y] == 1)
				glider[6].install( of, +PERIOD*i, -PERIOD*i, yFlag);
		}

		if( dots[dotShift( 2 -1 +2*refShift,shiftNum,xAreaSize)][y] == 1)
			glider[3].install( of,  +PERIOD*refShift, -PERIOD*refShift, yFlag);
		if( dots[dotShift( 3 -1 +2*refShift,shiftNum,xAreaSize)][y] == 1)
			glider[2].install( of,  +PERIOD*refShift,-PERIOD*refShift, yFlag);
		if( dots[dotShift( 5 -1 +4*refShift,shiftNum,xAreaSize)][y] == 1)
			glider[1].install( of, 0, 0, yFlag);

	}
	return 0;
}

