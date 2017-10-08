#include "stdafx.h"
#include "life_object.h"

using namespace std;
extern int errorNum;
extern int debugFlag;
extern string outputFileName;
const int X_MAX = 1024;


std::string OBJECT_PATH_PREFIX ="./objects/";

void LifeObject::Set( std::string f, int x, int y, int p, int d){
	fileNameRoot = f;
	xCentre = x;
	yCentre = y;
	phase = p;
	direction = d;

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

	char tempString[X_MAX];
	tempString[0] = '#';
	while( tempString[0] == '#') fgets( tempString, sizeof(tempString), inputFile);

	int eofFlag =0;

	while( eofFlag != 1){
		int x, y;
		sscanf( tempString, "%d %d", &x, &y);
		Coordinate pos(x - xCentre, y - yCentre);
		coordinates.push_back(pos);
		if( fgets( tempString, sizeof(tempString), inputFile) == NULL){
			eofFlag = 1;
		}
	}
	fclose( inputFile);
}

std::string LifeObject::genFileName( std::string path){
	std::string FileName;
	FileName = path;
	FileName += fileNameRoot + ".";
	FileName += std::to_string(phase) + ".";
	FileName += std::to_string(direction) + ".life";
	return FileName;
}

void LifeObject::install(int shiftX, int shiftY){
	shiftX -= xShift;
	shiftY -= yShift;
	for(Coordinate c: coordinates) {
		c.x += shiftX;
		c.y += shiftY;
		c.y *= yFlag;
		outputDots.push_back(c);
	}
}

void LifeObject::write() {
	std::string output = "#Life 1.06\n";
	sort(outputDots.begin(), outputDots.end());
	for( auto dot : outputDots ){
		output.append(dot.to_str());
	}

	/* opening output file */
	FILE *outputFile;
	outputFile = fopen( outputFileName.c_str(), "w");

	/* checking the file */
	if( outputFile == NULL){
		cout << "Can't open " << outputFileName << endl;
		errorNum++;
	}else{
		if( debugFlag != 0) cout << outputFileName << "is successfully opened" << endl;
	}

	fprintf( outputFile, "%s", output.c_str() );
	fclose( outputFile);
}
