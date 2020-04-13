#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glew.h>
#include <glfw3.h>

#include <vec3.hpp>
#include <mat4x4.hpp>
using namespace glm;

namespace classicminigraphics {
	bool begin(int width, int height, const char* title, bool fullscreen);
	void endEvents();
	void startEvents();

	extern GLFWwindow* window;
	extern float deltaTime;
	extern float lastFrameTime;

	extern float width;
	extern float height;
	extern float aspectDivider;

	extern float closeCamera;
	extern float farCamera;

	extern vec3 cameraPosition;
	mat4 viewMatrix();
	mat4 projectionMatrix();
}

#endif
