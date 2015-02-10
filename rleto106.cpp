#include "stdafx.h"
#include "rleto106.h"
#include "file_manage.h"
extern int debugFlag;

using namespace std;

int rleto106(int argc, char *argv[]){
	int outToFile = 1; /* 0 -> output to stream; 1 -> output to a file; */
	string outputFileName = "newfile.life";

	/* open the input file */
	FILE *rleFile;
	if( argc > 1){
		rleFile = fopen( argv[1], "r");
		if( rleFile == NULL){
			printf("Can't open %s. Try again.\n", argv[1]);
			return 1;
		}
	}else{
		cout << "Please input file rle file name." << endl;
		exit(1);
	}

	/* skipping lines starting with '#' */
	char string[100000];
	string[0] = '#';
	while( string[0] == '#') fgets( string, sizeof(string), rleFile);

	/* getting area size */
	int x = 3, y = 3;  	//the size of the area	
	if( string[0] == 'x') sscanf(string, "x = %d, y = %d", &x, &y);
	if( debugFlag == 1 ) printf( "x = %d, y = %d\n", x, y);
	/* not used */

	/* printf("Starting reading\n"); */
	if( outToFile == 1)
		outputFileInitialise( outputFileName.c_str(), "#Life 1.06\n");
	else
		printf("#Life 1.06\n");

	fgets( string, sizeof(string), rleFile);
	string[strlen(string) -1] = '_';
	if( debugFlag == 1) printf("Starting reading\n");
	if( debugFlag == 1) printf("string: %s %lu\n", string, strlen(string));

	int eofFlag = 0;
	while( eofFlag != 1 ){
		/* reading */
		static int yTemp = 0;
		static int xTemp = 0;
		int runCount = 1;

		if( sscanf( string, "%d", &runCount) == 1){ //number -> 1, not -> 0
			/* the letter is a number */
			if( debugFlag == 1) printf("%d: %d digit(s)\n", runCount, countDigits(runCount));
			char tag = string[0 +countDigits(runCount)];
			if( debugFlag == 1) printf("tag: %c\n", tag);
			stringShift( countDigits(runCount) + 1, string); //ignoring the number
			switch(tag){
				case 'o':
					for( int k = 0; k < runCount; k++){
						/* printf("%d %d\n", xTemp, yTemp); xTemp++; */
						outputLiveCell( outToFile, outputFileName.c_str(), xTemp, yTemp);
						xTemp++;
					}
					break;
				case 'b':
					if( debugFlag == 1) printf("b: dead cell\n");
					xTemp += runCount;
					break;
				case '$':
					yTemp+=runCount;
					if( debugFlag == 1) printf("%d lines are skipped\n",runCount );
					if( debugFlag == 1) printf("\nReading the line y = %d\n", yTemp);
					xTemp = 0;
					break;
				case '_': //reload mark
					fgets( string, sizeof(string), rleFile);
					string[strlen(string) -1] = '_';
					if(debugFlag == 1){
						printf("string reloaded\n");
						printf("stirng: %s %lu\n\n", string, strlen(string));
					}
					break;
				default:
					printf("%c:",tag);
					printf("ERROR: \"o\", \"b\", or \"$\" are expected right after a number\n");
					return 1;
			}

		}else{
			/* the letter is not a number */
			char tag = string[0];

			switch(tag){
				case 'o':
					outputLiveCell( outToFile, outputFileName.c_str(), xTemp, yTemp);
				case 'b':
					stringShift(1, string);
					xTemp++;
					break;
				case '$':
					xTemp = 0;
					yTemp++;
					if( debugFlag == 1) printf("\nReading the line y = %d\n", yTemp);
					stringShift(1, string);
					break;
				case '!':
					xTemp = x;
					yTemp = y;
					if( debugFlag == 1) printf("End of the file\n");
					eofFlag = 1;
					break;
				case '#':
				case '_':
					fgets( string, sizeof(string), rleFile);
					string[strlen(string) -1] = '_';
					if( debugFlag == 1){
						printf("string reloaded\n");
						printf("stirng: %s %lu\n\n", string, strlen(string));
						printf("%d\n", xTemp);
					}
					break;
				default:
					printf("Error: Not expected letter: %c\n", tag);
					return 1;
			}
		}


	}

	return 0;
}

int countDigits(int num){
	int digitsCounts = 0;
	while(num != 0){
		num = num/10;
		digitsCounts++;
	}
	return digitsCounts;
}

int stringShift(int shiftNum, char string[]){
	int i;
	/* printf("stringShift is called\n"); */
	for( i = 0; i < strlen(string) - shiftNum; i++){
		/* string[i] = 'o'; */
		string[i] = string [i + shiftNum];
		/* string[i + shiftNum]='\0'; */
	}
	string[i] = '_';

	/* printf("%ld\n", sizeof(string) ); */
	if(debugFlag == 1) printf("string: %s %lu\n", string, strlen(string));
	return 0;
}


int outputLiveCell( int outToFile, const char* of, int x, int y){
	if( outToFile == 0){
		printf("%d %d\n", x, y);
	}else{
		FILE *outputFile;
		outputFile = fopen( of, "a");
		fprintf( outputFile, "%d %d\n", x, y);
		fclose( outputFile);
	}
	return 0;
}
