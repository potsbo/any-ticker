#ifndef _LIFE_OBJECT_H_
#define _LIFE_OBJECT_H_
#include "coordinate.h"

class LifeObject{
	private:
		std::string genFileName( std::string path, std::string fileNameRoot, int phase, int direction);
		std::vector<Coordinate> coordinates;
	public:
		static Coordinate shift;
		static int yFlag;
		static std::vector<Coordinate> outputDots;
		void Set( std::string f, int x, int y, int p, int d);
		void install(int shiftX, int shiftY);
		static void write();
		static void addCoordinate(Coordinate c);
};

#endif // _LIFE_OBJECT_H_
