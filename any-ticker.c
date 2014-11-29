#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* for errors */
const int debugFlag = 0;
int errorNum = 0;

/* default settings; don't change */
const char LIVE_CELL = '*';
const char DEAD_CELL = '_';
const char *OBJECT_PATH_PREFIX ="./objects/";
const char *FONT_PATH_PREFIX="./fonts/";
const int X_MAX = 1024;
const int Y_MAX = 128;
const int S_SIZE = 128;
const int PERIOD = 23;

/* interesting parameters */
const int gunsDotShift = 5; // can't be less than 4
const int yUnit = 18;

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
int append( object type, char *of, int shiftX, int shiftY, int yDirection);

int installGliders( object *glider, int dots[X_MAX][Y_MAX], int xAreaSize, int delShift, int yAreaSize, char of[S_SIZE]);


int main(int argc, char *argv[]){
	
	/* default values: don't have to change here */
	const int xDefAreaSize = 41;
	const int yDefAreaSize = 11;
	const int extraEaters = 2;
	const double bannerSize = 1.5;
	const char *outputFileNameDef = "any-ticker.life";
	const char *fontNameDef = "golly";
	const char *messageDef = "golly";

	/* setting objects */
	/* {type, xCentre, yCentre, phase, direction} */
	object eat = {"eater", 4, -11, 0, 0};
	object dup = {"duplicator", -5, 88, 0, 0};
	object lws = {"lwssmaker", -83, 31, 0, 0};
	object ref = {"reflector", -22, 85, 0, 0,};
	object shp = {"ship", -77, 10, 0, 0,};
	object blk = {"block", -78, 9, 0, 0,};
	object glider[7] = {
		{"glider", -23, 68, 0, 1}, {"glider", -28, 53, 1, 2},
		{"glider", -47, 57, 1, 4}, {"glider", -36, 69, 3, 3},
		{"glider", -39, 45, 1, 3}, {"glider", -35, 79, 2, 1},
		{"glider", -50, 57, 2, 3}
	};

	/* setting output file */
	char of[S_SIZE];
	setString( "output file name", outputFileNameDef, of);

	/* set yAreaSize */
	int yAreaSize = setInt("y area size", yDefAreaSize);

	/* settig font name */
	char fontName[S_SIZE];
	setString( "font name", fontNameDef, fontName);
	
	/* setting ticker message */
	char message[S_SIZE];
	setString( "ticker message", messageDef, message);

	/* setting the dot map */
	printf("\nStart reading font file to set dot map\n");
	printf("Message length: %lu\n", strlen(message));
	int dots[X_MAX][Y_MAX]; 
	int xLeastAreaSize = 0;
	for( int i = 0; i < strlen(message); i++)
		xLeastAreaSize += dotMap( fontName, message[i], yAreaSize, dots, xLeastAreaSize);
	 /* checking space between the last letter and the first */
	if( letterSpaceCheck( dots, 0, xLeastAreaSize -1, yAreaSize) > 0) xLeastAreaSize++; 
	printf("Dot map created\n");

	/* calculating area size; xArea should be 4n + 5 (n >= 0) */
	printf("\nStart calculating x area size\n");
	printf("x least area size: %d\n", xLeastAreaSize);
	int xAreaSize = 5;
	/* set xAreaSize to proper value */
	while( xAreaSize < xLeastAreaSize) xAreaSize += 4;
	printf("Calculated x area size: %d\n", xAreaSize);

	/* set dots in blank space to zero */
	for(int i; i < yAreaSize; i++)
		for(int j; j < xAreaSize - xLeastAreaSize; j++) dots[xLeastAreaSize +j][i] = 0;


	/* output file initialisation */
	outputFileInitialise( of);

	/* putting guns, reflectors, and gliders */
	printf("\nStart installing objects\n");

	/* installing ships( temporary glider eater) */
	int gunNum = yAreaSize;
	int delMax = 0;
	for( int i = 0; i < gunNum; i++){
		int xShift = gunsDotShift *PERIOD *(i/2);// guns and reflectors shifted by this value
		int yShift = yUnit *(i/2);
		int yFlag = pow(-1, i); // make object upside down
		int uselessDots = 0;
		int y;
		if( (i%2) == 0) y = ( (gunNum -1) -i) /2; 
		else y = i /2 +(gunNum + 1) /2; 

		int cycle = ( gunsDotShift*(i/2) + xAreaSize -1) /xAreaSize;
		for( int j =dotShift(j,i,xAreaSize ); j < xAreaSize *cycle; j++){
			if( dots[j][y] == 1){
				uselessDots++;
			}
		}

		if( uselessDots > delMax)
			delMax = uselessDots;
		/* uselessDots %= 18; // for now, can't delete more than 18 gliders */
		/* errorNum += uselessDots / 18; */
		
		int shpNum = uselessDots / 2;
		int blkNum = uselessDots % 2;
		for( int i = 0; i < shpNum; i++)
			append( shp, of, -xShift - i *4, -yShift -i*4, yFlag);
		if( blkNum == 1)
			append( blk, of, -xShift - shpNum*4, -yShift - shpNum*4, yFlag);
	}
	if( debugFlag != 0) printf("delMax: %d\n", delMax);
	int delShift = delMax /18;

	/* guns and reflectors */
	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i); // make object upside down
		int xShift = gunsDotShift *PERIOD *(i/2);// guns and reflectors shifted by this value
		int yShift = yUnit *(i/2);
		int refShift = (xAreaSize -5) /4; // reflector shifted depending on xAreaSize
	
		/* guns */
		append( dup, of, -xShift -delShift *PERIOD, -yShift -delShift *PERIOD, yFlag);
		append( lws, of, -xShift, -yShift, yFlag);

		/* reflector */
		append( ref, of, - xShift +PERIOD*refShift -delShift *PERIOD, -yShift -PERIOD*refShift -delShift *PERIOD, yFlag);

	}
	printf("%d gun(s) and reflector(s) installed\n", gunNum);
	
	/* gliders */
	installGliders( glider, dots, xAreaSize, delShift, gunNum, of);

	/* putting eaters */
	int distance = 0;// distance between eaters and guns
	distance += (int)ceil((xAreaSize *PERIOD) *bannerSize /2) *2;
	distance += gunsDotShift *PERIOD * ((gunNum -1) /2) - ((gunNum -1) /2) % 2; //eaters shifted because of the number of guns
	int eaterNum = gunNum + abs(extraEaters);
	for( int i = 0; i < eaterNum; i++){
		int yFlag = pow( -1, ( i+3)/2);
		int negFlag = pow( -1, (i + 2)/2);
		append( eat, of, - distance, - negFlag * 2*yUnit * ( (i + 2)/4), yFlag);
	}
	printf("%d eaters installed\n", eaterNum);


	printf("\nEnd combining with %d error(s)\n", errorNum);
}

int outputFileInitialise( char *of){

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( of, "w");

	/* checking the file */
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", of);
		return 1;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", of);
	}

	/* initialisation */
	fprintf( outputFile, "#Life 1.06\n");

	/* close */
	fclose( outputFile);

	printf("\nInitialisation done\n");
	return 0;
}

int append( object type, char *of, int shiftX, int shiftY, int yDirection){
	
	/* generating file name */
	char inputFileName[S_SIZE];
	sprintf(inputFileName, "%s%s.%d.%d.life", OBJECT_PATH_PREFIX, type.fileNameRoot, type.phase, type.direction);
	
	/* opening input file */
	FILE *inputFile;
	inputFile = fopen( inputFileName, "r");

	/* checking the file */
	if( inputFile == NULL){
		printf("Can't open \"%s\". Try again.\nMake sure that object files must be in %s directory\n", inputFileName, OBJECT_PATH_PREFIX );
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", inputFileName );
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( of, "a");

	/* checking the file */
	if( outputFile == NULL){
		printf("Can't open \"%s\". Try again.\n", of);
		errorNum++;
		return 1;
	}else{
		if( debugFlag != 0) printf("\"%s\" is successfully opened\n", of);
	}

	if(debugFlag != 0)printf("start reading %s\n", inputFileName);
	char tempString[X_MAX];
	tempString[0] = '#';
	while( tempString[0] == '#') fgets( tempString, sizeof(tempString), inputFile);
	
	int eofFlag =0;
	shiftX -= type.xCentre;
	shiftY -= type.yCentre;
	while( eofFlag != 1){
		int xTemp, yTemp;
		sscanf( tempString, "%d %d", &xTemp, &yTemp);
		fprintf( outputFile, "%d %d\n", xTemp + shiftX, (yTemp + shiftY )*yDirection);
		if( fgets( tempString, sizeof(tempString), inputFile) == NULL){
			eofFlag = 1;
		}else{
			/* printf("%s", tempString); */
		}
	}
	if( debugFlag != 0) printf("end reading \"%s\"\n", inputFileName);
	fclose( inputFile);
	fclose( outputFile);

	return 0;
}

int dotMap( char *font, char letter, int size, int dots[X_MAX][Y_MAX], const int x){

	if( letter == 10){
		printf("Line feed skipped\n");
		return 0;
	}

	/* generating font file name */
	char fontFileName[S_SIZE];
	const char *lastSix = &font[strlen(font)-6];
	if( strcmp( lastSix, ".tfont") !=0 ) 
		sprintf( fontFileName, "%s%s.tfont",FONT_PATH_PREFIX, font);

	/* opening font file */
	FILE *fp;
	fp = fopen( fontFileName, "r");
	if( fp == NULL){
		printf("Can't open \"%s\". Try again.\nMake sure that font files must be in %s directory\n", fontFileName, FONT_PATH_PREFIX );
		errorNum++;
		return 0;
	}else{
		if(debugFlag != 0) printf("\"%s\" is successfully created\n", fontFileName);
	}

	/* searching letter data on the file */
	char stringTag[S_SIZE];
	sprintf( stringTag, "#%c%d#\n", letter, size);
	if( debugFlag != 0) printf( "stringTag: %s\n", stringTag);
	char tempString[S_SIZE] = "##";
	int findFlag = 0;
	// 0: searching // 1: found // -1: not found
	while( findFlag == 0){
		if( debugFlag != 0) printf("tempString: %s\n", tempString);

		if( fgets( tempString, sizeof(tempString), fp) == NULL){
			char *shortString[S_SIZE];
			printf("\"%c\" with size %d was not found on %s\n", letter, size, fontFileName);
			errorNum++;
			return 0;
		}else 
			if( strcmp( tempString, stringTag) == 0) findFlag = 1; //found
	}

	/* settig letter width */
	while( tempString[0] == '#' || tempString[0] == '\n')
		fgets( tempString, sizeof(tempString), fp);
	int letterWidth = strlen(tempString) -1;
	printf("%c.letterWidth: %d\n", letter, letterWidth);

	/* setting dot map */
	for(int i = 0; i < size; i++){
		for( int j = 0; j < letterWidth; j++){

			if( debugFlag != 0)printf("%c:", tempString[j]);

			/* initialisation */
			dots[j + x][i] = 0;

			/* mapping */
			if(tempString[j] == LIVE_CELL){
				dots[j + x][i] = 1;
				if( debugFlag != 0)printf("a new live cell prepared to be installed\n");
			}else if(tempString[j] == DEAD_CELL){
				dots[j + x][i] = 0;
				if( debugFlag != 0)printf("a new dead cell prepared to be installed\n");
			}else{
				printf("no expexted letter in %s:%c\n", fontFileName, tempString[j]);
				errorNum++;
			}
		}
		fgets( tempString, sizeof(tempString), fp);
	}

	/* checking space between previous one */
	int letterSpaceFlag = 0;
	if( x != 0) letterSpaceFlag = letterSpaceCheck( dots, x, x -1, size);

	/* shifting a letter if too close to previous one */
	if(letterSpaceFlag > 0){
		for(int i = 0; i < size; i++){
			for(int j = 0; j < letterWidth; j++)
				dots[x + letterWidth -j][i] = dots[x +letterWidth -j -1][i];
			dots[x][i] = 0;
		}
		letterWidth++;
	}


	fclose( fp);
	return letterWidth;
}

int setInt( char *label, const int defValue){
	
	/* prompt */
	printf("\nInput integer for %s ( default: %d): ", label, defValue);
	
	/* read */
	int inputInt = 0; char buf[S_SIZE];
	if( fgets( buf, sizeof buf,  stdin) != NULL ) sscanf(buf, "%d", &inputInt); 

	/* decision */
	int setInt = defValue;
	if( inputInt > 0) setInt = inputInt; else printf("No input. Use default.\n");

	/* end */
	printf("y area size: %d\n", setInt);
	return setInt;
}

int setString( char *label, const char *defValue, char *setString){

	/* prompt */
	printf("\nInput %s ( default: \"%s\"): ", label, defValue);

	/* read */
	char inputString[S_SIZE] = "";
	fgets( inputString, sizeof inputString, stdin);
	strcpy( setString, defValue);

	/* decision */
	if( inputString[0] != 0 && inputString[0] != 10)
		strcpy( setString, inputString);
	else printf("No input. Use default.\n");

	/* end */
	printf("%s: %s\n", label, setString);
	return 0;
}

int dotShift(int base, int shiftNum, int xAreaSize){
	return (base +xAreaSize*shiftNum -gunsDotShift*(shiftNum/2))%xAreaSize;
}

int letterSpaceCheck(int dots[X_MAX][Y_MAX], int x, int xTarget, int size){
	int letterSpaceFlag = 0;
	letterSpaceFlag += dots[x][0] *( dots[xTarget][0] +dots[xTarget][0 +1] );
	letterSpaceFlag += dots[x][size-1] *( dots[xTarget][size-1] +dots[xTarget][size -1]);
	for( int i = 0+1; i < size-1; i++){
		letterSpaceFlag += dots[x][i] *( dots[xTarget][i] +dots[xTarget][i -1] +dots[xTarget][i +1]);
	}
	if( letterSpaceFlag > 0) printf("Letter space inserted\n");
	return letterSpaceFlag;
}


int installGliders( object *glider, int dots[X_MAX][Y_MAX], int xAreaSize, int delShift, int gunNum, char of[S_SIZE]){

	for(int i = 0; i < gunNum; i++){
		int yFlag = pow(-1, i); // make object upside down
		int xShift = gunsDotShift *PERIOD *(i/2) + delShift *PERIOD;// guns, reflectors, and gliders shifted by this value
		int yShift = yUnit *(i/2) + delShift *PERIOD;
		int shiftNum = i;
		int refShift = (xAreaSize -5) /4; // reflector shifted depending on xAreaSize
		int y;
		if( (i%2) == 0) y = ( (gunNum -1) -i) /2; 
		else y = i /2 +(gunNum + 1) /2; 

		if( debugFlag != 0)	printf("i: %d, y: %d\n", i, y);
	
		/* gliders */
		for( int i = 0; i < refShift +1; i++){
			if( dots[dotShift(i*2,shiftNum,xAreaSize)][y] == 1)
				append( glider[0], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
			if( dots[dotShift(xAreaSize - 2 - 2*i,shiftNum,xAreaSize)][y] == 1)
				append( glider[4], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
		}

		for( int i = 0; i < refShift; i++){
			if( dots[dotShift( i*2 + 1,shiftNum,xAreaSize)][y] == 1)
				append( glider[5], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
			if( dots[dotShift( xAreaSize - 3 - 2*i,shiftNum,xAreaSize)][y] == 1)
				append( glider[6], of, -xShift +PERIOD*i, -yShift -PERIOD*i, yFlag);
		}

		if( dots[dotShift( 2 -1 +2*refShift,shiftNum,xAreaSize)][y] == 1)
			append( glider[3], of, -xShift + PERIOD*refShift, -yShift -PERIOD*refShift, yFlag);
		if( dots[dotShift( 3 -1 +2*refShift,shiftNum,xAreaSize)][y] == 1)
			append( glider[2], of, -xShift + PERIOD*refShift, -yShift -PERIOD*refShift, yFlag);
		if( dots[dotShift( 5 -1 +4*refShift,shiftNum,xAreaSize)][y] == 1)
			append( glider[1], of, -xShift, - yShift, yFlag);

	}
	return 0;
}
