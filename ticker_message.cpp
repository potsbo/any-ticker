#include "stdafx.h"
#include "typeset.h"
#include "ticker_message.h"

extern int X_DOT_SHIFT;
extern int PERIOD;
extern int bannerSize;

void TickerMessage::setDots(int dots[][256]){
	int x_min = 0;
	for( int i = 0; i < message.length(); i++)
		x_min += typeSetMapping(fontName.c_str(), message[i], yAreaSize, dots, x_min);
	xLeastAreaSize = x_min;

	/* checking space between the last letter and the first */
	if( letterSpaceCheck( dots, 0, xLeastAreaSize -1, yAreaSize) > 0) xLeastAreaSize++; 

	xAreaSize = 5; // because the minimun gun has 5 positions to have a glider 
	/* set xAreaSize to proper value */
	while( xAreaSize < xLeastAreaSize) xAreaSize += 4;

	/* set dots in blank space to zero */
	for(int y = 0; y < yAreaSize; y++)
		for(int x = 0; x < xAreaSize - xLeastAreaSize; x++)
			dots[xLeastAreaSize +x][y] = 0;
}

int TickerMessage::calculateDistance(){
	int distance = 4;// distance between eaters and guns
	while( distance < xAreaSize *PERIOD *bannerSize) distance += 4;
	distance += X_DOT_SHIFT *PERIOD * ((yAreaSize -1) /2);
	distance -= ((yAreaSize -1) /2) % 2; /* adjusting parity */
	return distance;
}
