#ifndef _ANY_TICKER_H_
#define _ANY_TICKER_H_

/* default settings; don't change */
extern const int S_SIZE;
extern const int Y_MAX;
extern const int X_MAX;
extern int errorNum;
extern std::string OBJECT_PATH_PREFIX;
extern int debugFlag;
using namespace std;

int any_ticker( int argc, char *argv[]);

/* calculate which glider is used for which dot */
int dotShift(int base, int shiftNum, int xAreaSize);

/* set a parameter from default value and user input */
int setInt( char *label, const int defValue);
int setString( char *label, const char *defValue, char *setString);

/* check space between two letters */
int letterSpaceCheck(int dots[][256], int x, int xTarget, int size);

/* objects like gliders or eaters */
class LifeObject{
	/* char fileNameRoot[256]; */
	/* filename must be "fileNameRoot.phase.direction.life" */
	public:
	std::string fileNameRoot;
	int xCentre; int yCentre; int phase; int direction;
	void Set( std::string f, int x, int y, int p, int d){
		fileNameRoot = f;
		xCentre = x;
		yCentre = y;
		phase = p;
		direction = d;
	}
	std::string genFileName( std::string path){
		std::string FileName;
		FileName = path;
		FileName += fileNameRoot + ".";
		FileName += std::to_string(phase) + ".";
		FileName += std::to_string(direction) + ".life";
		return FileName;
	}

	void install(const char *of, int shiftX, int shiftY, int yDirection){
	
		/* generating file name */
		/* char inputFileName[S_SIZE]; */
		string inputFileName = genFileName(OBJECT_PATH_PREFIX);
		/* sprintf(inputFileName, "%s%s.%d.%d.life", OBJECT_PATH_PREFIX, type.fileNameRoot, type.phase, type.direction); */

		/* opening input file */
		FILE *inputFile;
		inputFile = fopen( inputFileName.c_str(), "r");

		/* checking the file */
		if( inputFile == NULL){
			cout << "Can't open " << inputFileName;
			cout << "Try again." << endl;
			cout << "Make sure that object fils must be in " << OBJECT_PATH_PREFIX << endl;
			errorNum++;
		}else{
			if( debugFlag != 0) cout << inputFileName << "is successfully opened" << endl;
		}

		/* opening output file */
		FILE *outputFile;
		outputFile = fopen( of, "a");

		/* checking the file */
		if( outputFile == NULL){
			cout << "Can't open " << of << endl;
			errorNum++;
		}else{
			if( debugFlag != 0) cout << of << "is successfully opened" << endl;
		}

		char tempString[X_MAX];
		tempString[0] = '#';
		while( tempString[0] == '#') fgets( tempString, sizeof(tempString), inputFile);

		int eofFlag =0;
		shiftX -= xCentre;
		shiftY -= yCentre;
		while( eofFlag != 1){
			int xTemp, yTemp;
			sscanf( tempString, "%d %d", &xTemp, &yTemp);
			fprintf( outputFile, "%d %d\n", xTemp + shiftX, (yTemp + shiftY )*yDirection);
			if( fgets( tempString, sizeof(tempString), inputFile) == NULL){
				eofFlag = 1;
			}
		}
		fclose( inputFile);
		fclose( outputFile);

	}
};
// 0: no move // 1: South West // 2: South East // 3: North East // 4: North West

/* append object into output file */
/* int installObject( LifeObject type, const char *of, int shiftX, int shiftY, int yDirection); */

int installGliders( LifeObject *glider, int dots[][256], int xAreaSize, int delShift, int yAreaSize, const char *of);

#endif //_ANY_TICKER_H_
