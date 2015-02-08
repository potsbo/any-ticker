#include <iostream>
#include "file_convert.h"
#include "rleto106.h"

using namespace std;

int file_convert( int argc, char *argv[]){
	cout << "Note: Currently, we support only rle to life106 conversion." << endl;
	rleto106( argc, argv);

	return 0;
}
