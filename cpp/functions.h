#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "classicminiGraphics.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>

#include <sstream>
#include <fstream>

#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace glm;

namespace classicminiSystem {
	extern int WINDOWS;
	extern int MAC_OSX;
	extern int LINUX;
	extern int OS_UNKNOWN;

	string executeCommand(const char* command);
	int getOSType();
}

namespace savefiles {
	vector<string> readLines(const char* fileName);
	vector<string> splitComma(string used);
	bool fileExists(string name);
	vector<string> splitNewLine(string used);
}

namespace shader {
	int createShader(const char* filePath, GLenum shaderType);
	int createProgram(vector<int> shaders);

	void setVectorFour(int shader, const char* vecName, vec4 usedVec);
	void setMat4(int shader, const char* matrixName, mat4 usedMatrix);
	void setInt(int shader, const char* intName, int usedInt);
}

namespace gridMath {
	double tanSquared(double x);
	double reciprocalCos(double x);
	double sinSquared(double x);
	float bearing(vec2 one, vec2 two);

	vec2 OSGB36_ToLatLng(vec2 grid);
	vec2 latLngToGrid_OSGB36(vec2 latLng);

	vec2 latLngToGrid_WGS84(vec2 latLng);

	bool onScreen(mat4 projection, mat4 view, vec3 position);
}

namespace text {
	struct Character {
		GLuint textureID;
		ivec2 size;
		ivec2 bearing;
		GLuint advance;
	};

	extern GLuint textVAO, textVBO;
	extern int textShader;
	extern map<GLchar, Character> fontCharacters;

	void getFont(const char* fontPath, int fontSize);
	void begin(int fontSize);

	vec2 textMeasurements(string text, float size);
	void renderText(string text, vec3 position, float size, bool centered, vec4 textColour);
}

namespace texture {
	void begin();

	struct texture { 
		unsigned int textureId;
		int width, height;
		int channels;
		unsigned char* data;
		const char* name;
	};

	texture loadTexture(const char* filePath); // load texture from file and return a relevant texture class
	void enableTexture(texture usedTexture);
}

#endif 

