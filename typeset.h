#ifndef _TYPESET_H_
#define _TYPESET_H_
extern const int X_MAX, Y_MAX, S_SIZE;
extern const char *FONT_PATH_PREFIX;
extern int debugFlag, errorNum;
extern int letterSpaceCheck();
/* create dot map from font file */
int typeSetMapping( char *font, char letter, int size, int dots[X_MAX][Y_MAX], int xLeastAreaSize);
#endif
