#include "stdafx.h"
#include "life_object.h"

using namespace std;
extern int errorNum;
extern int debugFlag;
extern string outputFileName;
const int X_MAX = 1024;

std::string Coordinate::to_str() {
	std::string xs = std::to_string(x);
	std::string ys = std::to_string(y);
	return xs + " " + ys + "\n";
}

std::string OBJECT_PATH_PREFIX ="./objects/";

void LifeObject::Set( std::string f, int x, int y, int p, int d){
	fileNameRoot = f;
	xCentre = x;
	yCentre = y;
	phase = p;
	direction = d;
}

std::string LifeObject::genFileName( std::string path){
	std::string FileName;
	FileName = path;
	FileName += fileNameRoot + ".";
	FileName += std::to_string(phase) + ".";
	FileName += std::to_string(direction) + ".life";
	return FileName;
}

std::vector<Coordinate> LifeObject::install(int shiftX, int shiftY){
	shiftX -= xShift;
	shiftY -= yShift;

	/* generating file name */
	std::string inputFileName = genFileName(OBJECT_PATH_PREFIX);

	/* opening input file */
	FILE *inputFile;
	inputFile = fopen( inputFileName.c_str(), "r");

	/* checking the file */
	if( inputFile == NULL){
		cout << "Can't open " << inputFileName;
		cout << "Try again." << endl;
		cout << "Make sure that object files are in " << OBJECT_PATH_PREFIX << endl;
		errorNum++;
	}else{
		if( debugFlag != 0) cout << inputFileName << "is successfully opened" << endl;
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( outputFileName.c_str(), "a");

	/* checking the file */
	if( outputFile == NULL){
		cout << "Can't open " << outputFileName << endl;
		errorNum++;
	}else{
		if( debugFlag != 0) cout << outputFileName << "is successfully opened" << endl;
	}

	char tempString[X_MAX];
	tempString[0] = '#';
	while( tempString[0] == '#') fgets( tempString, sizeof(tempString), inputFile);

	std::vector<Coordinate> coordinates;
	int eofFlag =0;
	shiftX -= xCentre;
	shiftY -= yCentre;
	while( eofFlag != 1){
		int xTemp, yTemp;
		sscanf( tempString, "%d %d", &xTemp, &yTemp);
		int x = xTemp + shiftX;
		int y = (yTemp + shiftY) *yFlag;
		Coordinate pos(x, y);
		outputDots.push_back(pos);
		fprintf( outputFile, "%s", pos.to_str().c_str() );
		if( fgets( tempString, sizeof(tempString), inputFile) == NULL){
			eofFlag = 1;
		}
	}
	fclose( inputFile);
	fclose( outputFile);
	return coordinates;
}

