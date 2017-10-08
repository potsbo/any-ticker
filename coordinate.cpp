#include "stdafx.h"
#include "coordinate.h"

std::string Coordinate::to_str() {
	std::string xs = std::to_string(x);
	std::string ys = std::to_string(y);
	return xs + " " + ys + "\n";
}

