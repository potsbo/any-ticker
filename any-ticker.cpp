#include "stdafx.h"
#include "ticker_message.h"
#include "any-ticker.h"
#include "typeset.h"
#include "file_manage.h"
#include "life_object.h"
#include <algorithm>
#include <string>

using namespace std;

class InstallationPlaner {
	public:
		int xShiftForGunNumber(int i) {
			return X_DOT_SHIFT *PERIOD *(i/2);
		}
		int uselessDotsSizeForAGun(int dots[][256], int i, int y, int xAreaSize) {
			int cycle = ( X_DOT_SHIFT*(i/2) + xAreaSize -1) / xAreaSize; 
			int uselessDots = 0;
			for( int x =dotShift(0,i, xAreaSize ); x < xAreaSize *cycle; x++)
				if( dots[x % xAreaSize][y] == 1)
					uselessDots++;
			return uselessDots;
		}
		const int PERIOD = 23;
		const int X_DOT_SHIFT = 5; // can't be less than 4
		int dotShift(int base, int shiftNum, int xAreaSize){
			return (base +xAreaSize*shiftNum -X_DOT_SHIFT*(shiftNum/2))%xAreaSize;
		}
		int calculateDistance(double bannerSize, int xAreaSize, int yAreaSize){
			int distance = 4;// distance between eaters and guns
			while( distance < xAreaSize *PERIOD *bannerSize) distance += 4;
			distance += X_DOT_SHIFT *PERIOD * ((yAreaSize -1) /2);
			distance -= ((yAreaSize -1) /2) % 2; /* adjusting parity */
			return distance;
		}
};


/* interesting parameters */
const int X_DOT_SHIFT = 5; // can't be less than 4
const int Y_UNIT = 18;		// must be 18, otherwise cause bug, which should be fixed
const int S_SIZE = 256;

int LifeObject::xShift, LifeObject::yShift;

int any_ticker(int argc, char *argv[]){
	TickerMessage ticker("golly", "golly");

	/* default values: you don't have to change here */
	string outputFileName = "any-ticker.life";
	int xDefAreaSize = 41;	// useless variable
	int yDefAreaSize = 11;	// same as font size
	int extraEaters = 0;	// you can add extra eaters
	double bannerSize = 2.425;	// banner area is bannersize times longer than message
	int galaxyLess = 2;		// no. of galaxies is less than that of eaters by this
	int promptFlag = 0;

	int tag;
	while( ( tag = getopt( argc, argv, "df:m:o:ps:l:")) != -1){
		switch( tag){
			case 'd':
				debugFlag = 1;
				cout << "debugFlag: " << debugFlag << endl;
				break;
			case 'f':
				ticker.fontName = optarg;
				cout << "fontName: " << ticker.fontName << endl;
				break;
			case 'm':
				ticker.message = optarg;
				cout << "Message: " << ticker.message << endl;
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
				if( (ticker.yAreaSize = atoi(optarg)) != 0)
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
	int dots[1024][256]; 	// each dot
	ticker.setDots(dots);

	/* output file initialisation */
	outputFileInitialise( outputFileName.c_str(), "#Life 1.06\n");

	InstallationPlaner planer;

	/* installing ships( temporary glider eater) */
	int gunNum = ticker.yAreaSize;
	int delMax = 0;		// max number of useless dots(gliders) of each gun
	int adjustFlag = 1; /* for debug */
	if( adjustFlag == 1){
		int shipNum = 0;	// one ship can delete two dots(gliders)
		for( int i = 0; i < gunNum; i++){
			/* each row */
			LifeObject::xShift = X_DOT_SHIFT *planer.PERIOD *(i/2);
			// guns and reflectors shifted by this
			LifeObject::yShift = Y_UNIT *(i/2);
			int yFlag = pow(-1, i); // make object upside down
			int y = ( gunNum -yFlag *i +i%2)/2;
			int uselessDots = planer.uselessDotsSizeForAGun(dots, i, y, ticker.xAreaSize);

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
	}

	/* calculating where to put gliders and reflectors */
	int delShift = 0;
	while( delShift *planer.PERIOD + 41 < (delMax+1)/2 *4) delShift++;

	/* guns and reflectors */
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i);						// make object upside down
		LifeObject::xShift = X_DOT_SHIFT *planer.PERIOD *(i/2);	// guns and reflectors shifted by this
		LifeObject::yShift = Y_UNIT *(i/2);
		int refShift = (ticker.xAreaSize -5) /4;			// reflector shift depens on xAreaSize

		/* guns */
		dup.install( outputFileName.c_str(), -delShift *planer.PERIOD, -delShift *planer.PERIOD, yFlag);
		lws.install( outputFileName.c_str(), 0, 0, yFlag);

		/* reflectors */
		ref.install( outputFileName.c_str(), +planer.PERIOD*refShift -delShift *planer.PERIOD,
				-planer.PERIOD*refShift -delShift *planer.PERIOD, yFlag);
	}

	/* installing gliders */
	installGliders( glider, dots, ticker.xAreaSize, delShift, gunNum, outputFileName.c_str(), planer.PERIOD);

	/* installing eaters */
	//eaters shifted because of the number of guns
	int distance = planer.calculateDistance(bannerSize, ticker.xAreaSize, ticker.yAreaSize);
	int eaterNum = gunNum + abs(extraEaters);
	LifeObject::xShift = LifeObject::yShift = 0;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, (i+3)/2);
		int negFlag = pow( -1, (i+2)/2);
		eat.install( outputFileName.c_str(), -distance,
				-negFlag * 2*Y_UNIT * ( (i + 2)/4), yFlag);
	}

	/* installing galaxies (both right and left of eaters) */	
	int galaxyNum = max( gunNum -galaxyLess, 1);
	for( int i = -(galaxyNum + 1)/2 ; i < galaxyNum/2; i++){
		/* each row */
		int y = i + ( gunNum +1)/2;
		while(y < 0) y +=8;

		/* galaxies on the left of eaters */
		galaxy[y%8].install( outputFileName.c_str(), -distance, 18*i, 1);

		/* calculating which galaxy to have to make it a temporary eater */
		int firstLive = ticker.xAreaSize; 
		for(int x = 0; x < ticker.xAreaSize; x++){
			if(dots[x][y] == 1){
				firstLive = x;
				break;
			}
		}
		/* ( firstLive == xAreaSize) means there is no live cell in that row */

		/* installing the appropriate phase of galaxy */
		if( firstLive != ticker.xAreaSize){ 
			int genToGlx = distance *2;
			genToGlx += 229 +123; /* <Generations to first lwss> + <adjust num> */
			genToGlx += firstLive *planer.PERIOD *2;
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

	return 0;
}



int dotShift(int base, int shiftNum, int xAreaSize){
	return (base +xAreaSize*shiftNum -X_DOT_SHIFT*(shiftNum/2))%xAreaSize;
}

int installGliders( LifeObject *glider, int dots[][256], int xAreaSize, int delShift, int gunNum, const char *of, int PERIOD){

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

