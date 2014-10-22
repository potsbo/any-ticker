#include <stdio.h>
#include <string.h>

int stringShift(int shiftNum, char string[]);
int countDigits(int num);

int main(int argc, char *argv[]){
	/* char string[]="bo$2bo$3o!"; */
	/* printf("%d\n", argc); */
	char string[1000];
	FILE *rleFileName;
	rleFileName = fopen( argv[1], "r");
	if( rleFileName == NULL){
		printf("Can't open %s. Try again.\n", argv[1]);
		return 1;
	}
	int x = 3, y = 3;  	//the size of the area	
	fscanf(rleFileName, "x = %d, y = %d\n", &x, &y);
	fscanf(rleFileName, "%s", string);

	/* printf("Starting reading\n"); */
	/* printf("%s\n",string); */
	printf("#life 1.06\n");
	for(int yTemp = 0; yTemp < y; yTemp++){
		/* printf("\nReading the line y = %d\n", -yTemp); */
		for(int xTemp = 0; xTemp <= x;){
			/* printf("\n%s\n",string); */
			int runCount;

			if( sscanf(string, "%d", &runCount) == 1){
				/* the letter is a number */
				/* printf("%d\n", runCount); */
				stringShift(countDigits(runCount), string);
				char tag = string[0];
				/* printf("%c\n", tag); */
				for(int k = 0; k < runCount; k++){
					switch(tag){
						case 'o':
							/* printf("%d %d was added\n", xTemp, -yTemp); */
							printf("%d %d\n", xTemp, -yTemp);
							break;
						case 'b':
							break;
						default:
							printf("ERROR: Either o or b is expected right after a number");
							return 1;
					}
					xTemp++;
				}
				stringShift(1, string);
			}else{
				/* the letter is not a number */
				char tag = string[0];
				/* printf("%c\n", tag); */
				switch(tag){
					case 'o':
						/* printf("%d %d was added\n", xTemp, -yTemp); */
						printf("%d %d\n", xTemp, -yTemp);
						stringShift(1, string);
						break;
					case '$':
						stringShift(1, string);
						xTemp = x;
						/* printf("End of the line\n"); */
						break;
					case 'b':
						stringShift(1, string);
						break;
					case '!':
						yTemp = y;
						/* printf("End of the file"); */
						break;
					default:
						printf("Error: Not expected letter\n");
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
	}
	return 0;
}
