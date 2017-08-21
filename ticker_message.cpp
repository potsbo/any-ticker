#include "stdafx.h"
#include "typeset.h"
#include "ticker_message.h"

void TickerMessage::setDots(int dots[][256]){
	int x_min = 0;
	for( int i = 0; i < message.length(); i++)
		x_min += typeSetMapping(fontName.c_str(), message[i], yAreaSize, dots, x_min);
	xLeastAreaSize = x_min;

	/* checking space between the last letter and the first */
	if( letterSpaceCheck( dots, 0, xLeastAreaSize -1, yAreaSize) > 0) xLeastAreaSize++; 
}

