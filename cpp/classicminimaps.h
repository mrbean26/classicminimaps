#pragma once

#ifndef CLASSICMINIMAPS_H
#define CLASSICMINIMAPS_H

#include "functions.h"
#include "classicminiGraphics.h"
#include "mapInterface.h"

#include <vec2.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include <sstream>
#include <fstream>
#include <iostream>

using namespace glm;
using namespace std;

struct openName {
	string name = "";
	vec2 position = vec2(0.0f);
};

class mapSquare {
public:
	bool active = false;

	string fullFilePath = "";
	string justFileName = "";
	string gridSquareName = "";

	vector<vector<vec2>> shapes;
	vec2 minPoints = vec2(0.0f);
	vec2 maxPoints = vec2(0.0f);

	void load(string fileName);

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint size = 0;

	void loadOpenGLAttributes();

	// texts
	vector<openName> allNames;
	string namePath = "";

	void loadOpenNames();
};

namespace classicminimaps {
	void begin();
	void mainloop();

	void loadChunks();
	void render();

	void renderMapTexts();
	void loadTexts();
	bool objectWillBeOnScreen(vec2 objectPosition);
	void keyPresses(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mousePressed(GLFWwindow* window, int button, int action, int mods);

	extern vector<vec2> speedCameraLocations;
	extern GLuint speedVAO, speedVBO, speedSize;
	extern int speedShader;

	void loadSpeedCameras();
	void drawSpeedCameras();

	extern float scaleDivider;
	extern float height;
	extern int shaderProgram;
	extern float distanceToLoad;
	extern float rotationSearchInterval;

	extern float speedCameraSize;
	extern float cameraUpSpeed;
	extern float cameraForwardSpeed;
	extern float xShift;
	extern float yShift;

	extern vec4 textColour;
	extern float textSize;

	extern mapSquare rivers;
	extern vector<mapSquare> currentMapSquares;
	extern vector<openName> toRender;

	extern vector<vec2> speedCameraLocations;
	extern GLuint speedVAO, speedVBO, speedSize;
	extern int speedShader;
	extern texture::texture policeIconTexture;

	extern float speedCameraSize;
	extern float cameraUpSpeed;
	extern float cameraForwardSpeed;
	extern float xShift;
	extern float yShift;

	extern texture::texture yourIcon;
	extern float yourIconSize;
}

#endif 

