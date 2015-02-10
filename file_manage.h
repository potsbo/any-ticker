#ifndef _FILE_MANAGE_H_
#define _FILE_MANAGE_H_
class LifeFile {
	public:
		std::string fileName;
		void init( std::string initText);
		void putCell( int outToFile, int x, int y);
};
#endif // _FILE_MANAGE_H_
