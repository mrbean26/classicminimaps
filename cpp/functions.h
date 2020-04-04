#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include <vec4.hpp>
#include <fstream>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;
using namespace glm;

namespace savefiles {
	vector<string> readLines(const char* fileName);
	vector<string> splitComma(string used);
}

namespace shader {
	int createShader(const char* filePath, GLenum shaderType);
	int createProgram(vector<int> shaders);

	void setVectorFour(int shader, const char* vecName, vec4 usedVec);
	void setMat4(int shader, const char* matrixName, mat4 usedMatrix);
	void setInt(int shader, const char* intName, int usedInt);
}

namespace gridMath {
	vec2 gridToLatLng(vec2 grid);
	dvec2 latLngToGrid(dvec2 latLng);
}

namespace text {
	struct Character {
		GLuint textureID;
		ivec2 size;
		ivec2 bearing;
		GLuint advance;
	};

	extern map<GLchar, Character> fontCharacters;
	void getFont(const char* fontPath, int fontSize);
	void begin(int fontSize);
	vec2 textMeasurements(string text, float size);
	void renderText(string text, vec3 position, float size, bool centered, vec4 textColour);
}

#endif 

