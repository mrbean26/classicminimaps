#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <vec3.hpp>
using namespace glm;

namespace classicminigraphics {
	bool begin(int width, int height, const char* title);
	void endEvents();
	void startEvents();
	extern GLFWwindow* window;
	extern float deltaTime;

	extern float width;
	extern float height;
	extern float aspectDivider;

	extern float closeCamera;
	extern float farCamera;

	extern vec3 cameraPosition;
}

#endif
