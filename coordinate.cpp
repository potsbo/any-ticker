#include "stdafx.h"
#include "coordinate.h"

using namespace std;

string Coordinate::to_str() {
	string xs = to_string(x);
	string ys = to_string(y);
	return xs + " " + ys + "\n";
}

bool operator<( const Coordinate& left, const Coordinate& right ) {
	return left.x == right.x ? left.y < right.y : left.x < right.x;
}

Coordinate operator+( const Coordinate& left, const Coordinate& right ) {
	return Coordinate(left.x + right.x, left.y + right.y);
}

Coordinate operator-( const Coordinate& left, const Coordinate& right ) {
	return Coordinate(left.x - right.x, left.y - right.y);
}
