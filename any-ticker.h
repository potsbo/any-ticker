#ifndef _ANY_TICKER_H_
#define _ANY_TICKER_H_
#include "life_object.h"

int any_ticker( int argc, char *argv[]);

/* calculate which glider is used for which dot */
int dotShift(int base, int shiftNum, int xAreaSize);

/* set a parameter from default value and user input */
int setInt( char *label, const int defValue);
int setString( char *label, const char *defValue, char *setString);

/* check space between two letters */
int letterSpaceCheck(int dots[][256], int x, int xTarget, int size);

int installGliders( LifeObject *glider, int dots[][256], int xAreaSize, int delShift, int yAreaSize, const char *of, int PERIOD);

#endif //_ANY_TICKER_H_
