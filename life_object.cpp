#include "stdafx.h"
#include "life_object.h"

using namespace std;
extern int errorNum;
extern int debugFlag;
extern string outputFileName;
const int X_MAX = 1024;


string OBJECT_PATH_PREFIX ="./objects/";

void LifeObject::Set(string f, int xCentre, int yCentre, int p, int d){
	Coordinate centre(xCentre, yCentre);
	/* generating file name */
	string inputFileName = genFileName(OBJECT_PATH_PREFIX, f, p, d);

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

	bool finished = false;

	while(!finished){
		int x, y;
		sscanf( tempString, "%d %d", &x, &y);
		Coordinate pos(x,y);
		coordinates.push_back(pos - centre);
		if( fgets( tempString, sizeof(tempString), inputFile) == NULL){
			finished = true;
		}
	}
	fclose( inputFile);
}

string LifeObject::genFileName(string path, string fileNameRoot, int phase, int direction){
	string FileName;
	FileName = path;
	FileName += fileNameRoot + ".";
	FileName += to_string(phase) + ".";
	FileName += to_string(direction) + ".life";
	return FileName;
}

void LifeObject::install(int shiftX, int shiftY){
	Coordinate installationShift = Coordinate(shiftX, shiftY);
	LifeObject::install(installationShift);
}

void LifeObject::install(Coordinate shiftVec) {
	for(Coordinate c: coordinates) {
		Coordinate p = c + shiftVec -shift;
		p.y *= yFlag;
		outputDots.push_back(p);
	}
}

void LifeObject::write() {
	string output = "#Life 1.06\n";
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

void LifeObject::addCoordinate(Coordinate c) {
	outputDots.push_back(c);
}
