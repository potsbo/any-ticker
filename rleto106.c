#include <stdio.h>
#include <string.h>

int stringShift(int shiftNum, char string[]);
int countDigits(int num);

int main(int argc, char *argv[]){
	/* printf("%d\n", argc); */
	char string[100000];
	FILE *rleFileName;
	rleFileName = fopen( argv[1], "r");
	if( rleFileName == NULL){
		printf("Can't open %s. Try again.\n", argv[1]);
		return 1;
	}

	/* skipping lines starting with '#' */
	string[0] = '#';
	while( string[0] == '#'){
		fgets( string, sizeof(string), rleFileName);
	}

	/* getting are size */
	int x = 3, y = 3;  	//the size of the area	
	if( string[0] == 'x'){
		sscanf(string, "x = %d, y = %d", &x, &y);
	}
	/* printf("%s", string); */
	/* fscanf(rleFileName, "x = %d, y = %d\n", &x, &y); */
	printf( "x = %d, y = %d\n", x, y);

	/* printf("Starting reading\n"); */
	printf("#Life 1.06\n");
	int eofFlag = 0;
	while( eofFlag != 1){
		fgets( string, sizeof(string), rleFileName);
		int charCount = 0;
		printf("%s\n",string);
		for( int yTemp = 0; yTemp < y; yTemp++){
			printf("\nReading the line y = %d\n", -yTemp);
			for( int xTemp = 0; xTemp <= x;){
				/* printf("\n%s\n",string); */
				int runCount = 1;
				printf("charCount:%d\n", charCount);

				if( sscanf( string, "%d", &runCount) == 1){ //cause of problem
					/* the letter is a number */
					printf("%d: %d digits\n", runCount, countDigits(runCount));
					charCount+=countDigits(runCount);
					/* stringShift(countDigits(runCount), string); */
					char tag = string[charCount];
					printf("tag:%c\n", tag);
					for( int k = 0; k < runCount; k++){
						switch(tag){
							case 'o':
								/* printf("%d %d was added\n", xTemp, -yTemp); */
								printf("%d %d\n", xTemp, -yTemp);
								break;
							case 'b':
								break;
							case '$':
								yTemp+=runCount;
								break;
							default:
								printf("%c:",tag);
								printf("ERROR: Either o or b is expected right after a number\n");
								return 1;
						}
						xTemp++;
					}
					charCount++;
					/* stringShift(1, string); */
					for(int i = 0; i < runCount -1; i++){
						char ignore;
						scanf( string, "%1c", &ignore);
						printf("%c is ignored", ignore);
					}

				}else{
					/* the letter is not a number */
					char tag = string[charCount];
					printf("tag:%c\n", tag);
					switch(tag){
						case 'o':
							/* printf("%d %d was added\n", xTemp, -yTemp); */
							printf("%d %d\n", xTemp, -yTemp);
							charCount++;
							/* stringShift(1, string); */
							break;
						case '$':
							/* stringShift(1, string); */
							charCount++;
							xTemp = x;
							/* printf("End of the line\n"); */
							break;
						case 'b':
							/* stringShift(1, string); */
							charCount++;
							break;
						case '!':
							xTemp = x;
							yTemp = y;
							/* printf("End of the file"); */
							eofFlag = 1;
							break;
						case '#':
							break;
						default:
							printf("Error: Not expected letter\n");
							return 1;
					}
					xTemp++;
				}

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
	}
	/* printf("%ld\n", sizeof(string) ); */
	printf("%s\n", string);
	return 0;
}
