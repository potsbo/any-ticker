#include "stdafx.h"
extern int S_SIZE;

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
	char inputString[S_SIZE];
	fgets( inputString, sizeof inputString, stdin);

	/* removing line feed */
	if(inputString[strlen(inputString) -1] == 10)
		inputString[strlen(inputString) -1] = '\0';
	strcpy( setString, defValue);

	/* decision */
	if( inputString[0] != 0 && inputString[0] != 10)
		strcpy( setString, inputString);
	else printf("No input. Use default.\n");

	/* end */
	printf("%s: %s\n", label, setString);
	return 0;
}

