#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>

#include <vec4.hpp>
#include <fstream>
#include <mat4x4.hpp>
#include <gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

vector<string> readLines(const char* fileName);

int createShader(const char* filePath, GLenum shaderType);
int createProgram(vector<int> shaders);

void setVectorFour(int shader, const char* vecName, vec4 usedVec);
void setMat4(int shader, const char* matrixName, mat4 usedMatrix);

vec2 gridToLatLng(vec2 grid);
vec2 latLngToGrid(vec2 latLng);

#endif 

