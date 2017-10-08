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
		int uselessDotsSizeForAGun(int i, int y) {
			int cycle = ( X_DOT_SHIFT*(i/2) + xAreaSize -1) / xAreaSize; 
			int uselessDots = 0;
			for( int x =dotShift(0,i); x < xAreaSize *cycle; x++)
				if( dots[x % xAreaSize][y] == 1)
					uselessDots++;
			return uselessDots;
		}
		static const int PERIOD = 23;
		int dotShift(int base, int shiftNum){
			return (base +xAreaSize*shiftNum -X_DOT_SHIFT*(shiftNum/2))%xAreaSize;
		}
		int calculateDistance(double bannerSize){
			int distance = 4;// distance between eaters and guns
			while( distance < xAreaSize *PERIOD *bannerSize) distance += 4;
			distance += X_DOT_SHIFT *PERIOD * ((yAreaSize -1) /2);
			distance -= ((yAreaSize -1) /2) % 2; /* adjusting parity */
			return distance;
		}
		InstallationPlaner(string message, string fontName, int fontSize) {
			TickerMessage ticker(message, fontName, fontSize);
			ticker.setDots(dots);
			xAreaSize = ticker.xAreaSize;
			yAreaSize = ticker.yAreaSize;
		}

		void plan() {
			calculate_offset();
		}

		int delMax(){
			int m = 0;		// max number of useless dots(gliders) of each gun
			for( int i = 0; i < yAreaSize; i++){
				int yFlag = pow(-1, i); // make object upside down
				int y = ( yAreaSize -yFlag *i +i%2)/2;
				int uselessDots = uselessDotsSizeForAGun(i, y);
				m = max( uselessDots, m);
			}
			return m;
		}

		int delShift(){
			int s = 0;
			while( s *PERIOD + 41 < (delMax()+1)/2 *4) s++;
			return s;
		}
		void calculate_offset() {
			int s = delShift();
			offset = s * PERIOD;
		}
		int refShift() {
			return (xAreaSize -5) /4;			// reflector shift depens on xAreaSize
		}
		int xRefShift() {
			return PERIOD * refShift();
		}
		int dots[1024][256];
		void plan(int x, int y){
			xAreaSize = x;
			yAreaSize = y;
		}
		int xAreaSize;
		int yAreaSize;
		int offset;
	private:
		static const int X_DOT_SHIFT = 5; // can't be less than 4
};


/* interesting parameters */
const int Y_UNIT = 18;		// must be 18, otherwise cause bug, which should be fixed
const int S_SIZE = 256;

int LifeObject::xShift, LifeObject::yShift, LifeObject::yFlag;
std::vector<Coordinate> LifeObject::outputDots;
string outputFileName = "any-ticker.life";

int any_ticker(int argc, char *argv[]){
	/* default values: you don't have to change here */
	int xDefAreaSize = 41;	// useless variable
	int yDefAreaSize = 11;	// same as font size
	int extraEaters = 0;	// you can add extra eaters
	double bannerSize = 2.425;	// banner area is bannersize times longer than message
	int galaxyLess = 2;		// no. of galaxies is less than that of eaters by this
	int promptFlag = 0;

	string fontName = "golly";
	string message = "golly";
	int fontSize = 11;

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
				if( (fontSize = atoi(optarg)) != 0)
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
	InstallationPlaner planer(message, fontName, fontSize);
	planer.plan();

	/* output file initialisation */
	outputFileInitialise( outputFileName.c_str(), "#Life 1.06\n");

	/* installing ships( temporary glider eater) */
	for( int i = 0; i < planer.yAreaSize; i++){
		/* each row */
		LifeObject::xShift = planer.xShiftForGunNumber(i);
		// guns and reflectors shifted by this
		LifeObject::yShift = Y_UNIT *(i/2);
		int yFlag = LifeObject::yFlag = pow(-1, i); // make object upside down
		int y = ( planer.yAreaSize -yFlag *i +i%2)/2;
		int uselessDots = planer.uselessDotsSizeForAGun(i, y);

		int shpNum = uselessDots /2; 	// one ship deletes 2 gliders
		int blkNum = uselessDots %2;	// one block deletes 1 glider
		for( int i = 0; i < shpNum; i++)
			shp.install(-i*4, -i*4);
		if( blkNum == 1)
			blk.install(-shpNum*4, -shpNum*4);
		/* end of a row */
	}

	/* guns and reflectors */
	for(int i = 0; i < planer.yAreaSize; i++){
		int yFlag = LifeObject::yFlag = pow(-1, i);
		LifeObject::xShift = planer.xShiftForGunNumber(i) - planer.offset;
		LifeObject::yShift = Y_UNIT *(i/2) - planer.offset;

		/* guns */
		dup.install(0, 0);
		lws.install(planer.offset, planer.offset);

		/* reflectors */
		ref.install(+planer.xRefShift(), -planer.xRefShift());
	}

	for(int i = 0; i < planer.yAreaSize; i++){
		int yFlag = LifeObject::yFlag = pow(-1, i); // make object upside down
		LifeObject::xShift = planer.xShiftForGunNumber(i) + planer.offset;// gliders shifted by this
		LifeObject::yShift = Y_UNIT *(i/2) + planer.offset;
		int shiftNum = i;
		int y = ( planer.yAreaSize -yFlag *i +i%2)/2;

		/* gliders */
		for( int i = 0; i < planer.refShift() +1; i++){
			if( planer.dots[planer.dotShift(i*2,shiftNum)][y] == 1)
				glider[0].install(+planer.PERIOD*i, -planer.PERIOD*i);
			if( planer.dots[planer.dotShift(planer.xAreaSize - 2 - 2*i,shiftNum)][y] == 1)
				glider[4].install(+planer.PERIOD*i, -planer.PERIOD*i);
		}

		for( int i = 0; i < planer.refShift(); i++){
			if( planer.dots[planer.dotShift( i*2 + 1,shiftNum)][y] == 1)
				glider[5].install(+planer.PERIOD*i, -planer.PERIOD*i);
			if( planer.dots[planer.dotShift( planer.xAreaSize - 3 - 2*i,shiftNum)][y] == 1)
				glider[6].install(+planer.PERIOD*i, -planer.PERIOD*i);
		}

		if( planer.dots[planer.dotShift( 2 -1 +2*planer.refShift(),shiftNum)][y] == 1)
			glider[3].install(+planer.xRefShift(), -planer.xRefShift());
		if( planer.dots[planer.dotShift( 3 -1 +2*planer.refShift(),shiftNum)][y] == 1)
			glider[2].install(+planer.PERIOD*planer.refShift(),-planer.xRefShift());
		if( planer.dots[planer.dotShift( 5 -1 +4*planer.refShift(),shiftNum)][y] == 1)
			glider[1].install(0, 0);

	}

	/* installing eaters */
	//eaters shifted because of the number of guns
	int distance = planer.calculateDistance(bannerSize);
	int eaterNum = planer.yAreaSize + abs(extraEaters);
	LifeObject::xShift = LifeObject::yShift = 0;
	for( int i = 0; i < eaterNum; i++){
		int yFlag = LifeObject::yFlag = pow( -1, (i+3)/2);
		int negFlag = pow( -1, (i+2)/2);
		eat.install(-distance, -negFlag * 2*Y_UNIT * ( (i + 2)/4));
	}

	LifeObject::yFlag = 1;
	/* installing galaxies (both right and left of eaters) */	
	int galaxyNum = max( planer.yAreaSize -galaxyLess, 1);
	for( int i = -(galaxyNum + 1)/2 ; i < galaxyNum/2; i++){
		/* each row */
		int y = i + ( planer.yAreaSize +1)/2;
		while(y < 0) y +=8;

		/* galaxies on the left of eaters */
		galaxy[y%8].install(-distance, 18*i);

		/* calculating which galaxy to have to make it a temporary eater */
		int firstLive = planer.xAreaSize;
		for(int x = 0; x < planer.xAreaSize; x++){
			if(planer.dots[x][y] == 1){
				firstLive = x;
				break;
			}
		}
		/* ( firstLive == xAreaSize) means there is no live cell in that row */

		/* installing the appropriate phase of galaxy */
		if( firstLive != planer.xAreaSize){
			int genToGlx = distance *2;
			genToGlx += 229 +123; /* <Generations to first lwss> + <adjust num> */
			genToGlx += firstLive *planer.PERIOD *2;
			/* actually useless because (firstLive *PERIOD *2) %8 = 0 */
			genToGlx += firstLive *4;
			genToGlx += planer.delShift() *4;
			if( ( (y + ( planer.yAreaSize+1)/2)%2) %2 != 0)
				/* want to make this simple */
				galaxy[(genToGlx)%8].install(-distance+24, 18*i);
			else
				galaxy[(genToGlx+6)%8].install(-distance+23, 18*i);
		}
		/* end of a row */
	}

	LifeObject::write();

	return 0;
}
