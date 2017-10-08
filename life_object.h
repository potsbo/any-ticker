#ifndef _LIFE_OBJECT_H_
#define _LIFE_OBJECT_H_

class Coordinate{
	public:
		Coordinate(int newX, int newY) {
			x = newX;
			y = newY;
		}
		int x;
		int y;
		std::string to_str();
};

class LifeObject{
	private:
		std::string fileNameRoot;
		int xCentre; int yCentre; int phase; int direction;
		std::string genFileName( std::string path);
	public:
		static int xShift, yShift, yFlag;
		static std::vector<Coordinate> outputDots;
		void Set( std::string f, int x, int y, int p, int d);
		std::vector<Coordinate> install(int shiftX, int shiftY);
};

#endif // _LIFE_OBJECT_H_
