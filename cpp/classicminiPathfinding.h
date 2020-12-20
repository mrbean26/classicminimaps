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
	extern GLuint pathVAO;
	extern GLuint pathVBO;
	extern GLuint pathSize;

	extern bool finishedPathfind;
	extern bool startedFinding;
	extern bool updatedOpenGLAttributes;

	string findParentSquare(string point);
	string getSquareAverage(string squareCode);

	extern vector<float> pathVertices;
	void findAndLoadPath(string startPosition, string endPosition);
	void updateOpenGLAttributes(); // dont initialise openGl objects on a separate thread
	void startPostcodeRoute();
	void drawPath();

	void mainloop();
}

namespace aStar {
	vector<vec2> aStarFloat(string startPosition, string endPosition, map < string, vector<string>> connectionDictionary);
	vector<string> aStarString(string startPosition, string endPosition, map < string, vector<string>> connectionDictionary);
	vector<vector<string>> createLargerSquarePath(string startPosition, string endPosition);
	vector<vec2> returnFullPath(string startPosition, string endPosition);
}


#endif