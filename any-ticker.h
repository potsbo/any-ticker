#ifndef _ANY_TICKER_H_
#define _ANY_TICKER_H_

int any_ticker( int argc, char *argv[]);

/* calculate which glider is used for which dot */
int dotShift(int base, int shiftNum, int xAreaSize);

/* set a parameter from default value and user input */
int setInt( char *label, const int defValue);
int setString( char *label, const char *defValue, char *setString);

/* check space between two letters */
int letterSpaceCheck(int dots[][256], int x, int xTarget, int size);

class LifeObject{
	private:
		std::string fileNameRoot;
		int xCentre; int yCentre; int phase; int direction;
		string genFileName( string path);
	public:
		static int xShift, yShift;
		void Set( string f, int x, int y, int p, int d);
		void install( const char *of, int shiftX, int shiftY, int yDirection);
};
int LifeObject::xShift, LifeObject::yShift;


#endif //_ANY_TICKER_H_
