#include "stdafx.h"
#include "coordinate.h"

std::string Coordinate::to_str() {
	std::string xs = std::to_string(x);
	std::string ys = std::to_string(y);
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
