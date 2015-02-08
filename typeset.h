#ifndef _TYPESET_H_
#define _TYPESET_H_
extern const int X_MAX, Y_MAX, S_SIZE;
extern const char *FONT_PATH_PREFIX;
extern int debugFlag, errorNum;
/* create dot map from font file */
int typeSetMapping( const char *font, char letter, int size, int dots[1024][256], int xLeastAreaSize);
int letterSpaceCheck(int dots[1024][256], int x, int xTarget, int size);
#endif
