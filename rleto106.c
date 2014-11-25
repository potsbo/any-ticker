#include <stdio.h>
#include <string.h>

int stringShift(int shiftNum, char string[]);
int countDigits(int num);
int debugFlag = 0;

int main(int argc, char *argv[]){
	if(debugFlag == 1) printf("argc: %d\n", argc);

	/* open the input file */
	FILE *rleFileName;
	rleFileName = fopen( argv[1], "r");
	if( rleFileName == NULL){
		printf("Can't open %s. Try again.\n", argv[1]);
		return 1;
	}

	/* skipping lines starting with '#' */
	char string[100000];
	string[0] = '#';
	while( string[0] == '#'){ fgets( string, sizeof(string), rleFileName); }

	/* getting area size */
	int x = 3, y = 3;  	//the size of the area	
	if( string[0] == 'x'){ sscanf(string, "x = %d, y = %d", &x, &y); }
	if( debugFlag == 1){ printf( "x = %d, y = %d\n", x, y);}

	/* printf("Starting reading\n"); */
	printf("#Life 1.06\n");
		fgets( string, sizeof(string), rleFileName);
		string[strlen(string) -1] = '@';
		if( debugFlag == 1) printf("Starting reading\n");
		if( debugFlag == 1) printf("string: %s %lu\n", string, strlen(string));

			for( int yTemp = 0; yTemp < y; yTemp++){
				/* reading a y line */
				if( debugFlag == 1) printf("\nReading the line y = %d\n", -yTemp);
				for( int xTemp = 0; xTemp <= x;){
					/* printf("\n%s\n",string); */
					int runCount = 1;

					if( sscanf( string, "%d", &runCount) == 1){ 
						/* the letter is a number */
						if( debugFlag == 1){ printf("%d: %d digit(s)\n", runCount, countDigits(runCount)); }
						char tag = string[0 +countDigits(runCount)];
						if( debugFlag == 1) printf("tag: %c\n", tag);
						switch(tag){
							case 'o':
								for( int k = 0; k < runCount; k++){ printf("%d %d\n", xTemp, -yTemp); xTemp++; }
								break;
							case 'b':
								if( debugFlag == 1) printf("b: dead cell\n");
								for( int k = 0; k < runCount; k++){ xTemp++;}
								break;
							case '$':
								yTemp+=runCount;
								if( debugFlag ==1) printf("%d lines are skipped\n",runCount );
								if( debugFlag == 1) printf("\nReading the line y = %d\n", -yTemp);
								break;
							case '@': //reload mark
								fgets( string, sizeof(string), rleFileName);
								string[strlen(string) -1] = '@';
								if(debugFlag == 1){
									printf("string reloaded\n");
									printf("stirng: %s %lu\n\n", string, strlen(string));
								}
								break;
							default:
								printf("%c:",tag);
								printf("ERROR: Either o or b is expected right after a number\n");
								return 1;
						}
						stringShift( countDigits(runCount) + 1, string); //ignoring the number

					}else{
						/* the letter is not a number */
						char tag = string[0];
						stringShift(1, string);
						/* printf("tag:%c\n", tag); */
						switch(tag){
							case 'o':
								/* printf("%d %d was added\n", xTemp, -yTemp); */
								printf("%d %d\n", xTemp, -yTemp);
								/* stringShift(1, string); */
								break;
							case '$':
								/* stringShift(1, string); */
								xTemp = 0;
								yTemp++;
								if( debugFlag == 1) printf("\nReading the line y = %d\n", -yTemp);
								/* printf("End of the line\n"); */
								break;
							case 'b':
								/* stringShift(1, string); */
								break;
							case '!':
								xTemp = x;
								yTemp = y;
								if(debugFlag == 1){ printf("End of the file\n");}
								break;
							case '#':
								break;
							case '@':
								fgets( string, sizeof(string), rleFileName);
								string[strlen(string) -1] = '@';
								if(debugFlag == 1){
									printf("string reloaded\n");
									printf("stirng: %s %lu\n\n", string, strlen(string));
								}
								break;
							default:
								printf("Error: Not expected letter: %c\n", tag);
								return 1;
						}
						xTemp++;
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
	string[i] = '@';

	/* printf("%ld\n", sizeof(string) ); */
	if(debugFlag == 1) printf("string: %s %lu\n", string, strlen(string));
	return 0;
}
