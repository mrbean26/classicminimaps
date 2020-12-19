#pragma once

#ifndef CLASSICMINIPATHFINDING_H
#define CLASSICMINIPATHFINDING_H

#include "functions.h"

#include <glm.hpp>

#include <string>
#include <map>

using namespace std;
using namespace glm;

namespace general {
	float stringDistance(string positionOne, string positionTwo);
	map<string, vector<string>> dictionaryFromConnectionFile(string connectionFilename);
	string findNearestPoint(string point, string connectionFileFilename);
}

namespace specific {
	string findParentSquare(string point);
	string getSquareAverage(string squareCode);

	void findAndLoadPath(string startPosition, string endPosition);
	void drawPath();
}

namespace aStar {
	vector<vec2> aStarFloat(string startPosition, string endPosition, map < string, vector<string>> connectionDictionary);
	vector<string> aStarString(string startPosition, string endPosition, map < string, vector<string>> connectionDictionary);
	vector<vector<string>> createLargerSquarePath(string startPosition, string endPosition);
	vector<vec2> returnFullPath(string startPosition, string endPosition);
}


#endif