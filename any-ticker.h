#ifndef _ANY_TICKER_H_
#define _ANY_TICKER_H_

/* default settings; don't change */
extern const int S_SIZE;
extern const int Y_MAX;

int any_ticker( int argc, char *argv[]);

/* calculate which glider is used for which dot */
int dotShift(int base, int shiftNum, int xAreaSize);

/* set a parameter from default value and user input */
int setInt( char *label, const int defValue);
int setString( char *label, const char *defValue, char *setString);

/* check space between two letters */
int letterSpaceCheck(int dots[][256], int x, int xTarget, int size);

/* objects like gliders or eaters */
typedef struct {
	char fileNameRoot[256];
	/* filename must be "fileNameRoot.phase.direction.life" */
	int xCentre; int yCentre; int phase; int direction;
	// 0: no move // 1: South West // 2: South East // 3: North East // 4: North West
} object;

/* append object into output file */
int installObject( object type, const char *of, int shiftX, int shiftY, int yDirection);

int installGliders( object *glider, int dots[][256], int xAreaSize, int delShift, int yAreaSize, const char *of);

#endif //_ANY_TICKER_H_
