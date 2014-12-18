#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* for errors */
int debugFlag = 0;
int errorNum = 0;

/* default settings; don't change */
const char LIVE_CELL = '*';
const char DEAD_CELL = '_';
const char *OBJECT_PATH_PREFIX ="./objects/";
const char *FONT_PATH_PREFIX="./fonts/";
const int X_MAX = 1024;
const int Y_MAX = 128;
const int S_SIZE = 256;
const int PERIOD = 23;

/* interesting parameters */
const int X_DOT_SHIFT = 5; // can't be less than 4
const int Y_UNIT = 18;		// must be 18, otherwise cause bug, which should be fixed

/* initialise output file */
int outputFileInitialise( char *of);

/* calculate which glider is used for which dot */
int dotShift(int base, int shiftNum, int xAreaSize);

/* create dot map from font file */
int dotMap( char *font, char letter, int size, int dots[X_MAX][Y_MAX], int xLeastAreaSize);

/* set a parameter from default value and user input */
int setInt( char *label, const int defValue);
int setString( char *label, const char *defValue, char *setString);

/* check space between two letters */
int letterSpaceCheck(int dots[X_MAX][Y_MAX], int x, int xTarget, int size);

/* objects like gliders or eaters */
typedef struct {
	char fileNameRoot[S_SIZE];
	/* filename must be "fileNameRoot.phase.direction.life" */
	int xCentre; int yCentre; int phase; int direction;
	// 0: no move // 1: South West // 2: South East // 3: North East // 4: North West
} object;

/* append object into output file */
int installObject( object type, char *of, int shiftX, int shiftY, int yDirection);

int installGliders( object *glider, int dots[X_MAX][Y_MAX], int xAreaSize, int delShift, int yAreaSize, char of[S_SIZE]);

int max( int a, int b){
	if(a > b) return a;
	else return b;
}
