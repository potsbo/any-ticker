class LifeObject{
	private:
		std::string fileNameRoot;
		int xCentre; int yCentre; int phase; int direction;
		std::string genFileName( std::string path);
	public:
		static int xShift, yShift;
		void Set( std::string f, int x, int y, int p, int d);
		void install( const char *of, int shiftX, int shiftY, int yDirection);
};
