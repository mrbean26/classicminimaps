#pragma once

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace classicminigraphics {
	bool begin(int width, int height, const char* title);
	void endEvents();
	void startEvents();
	extern GLFWwindow* window;
	extern float deltaTime;
}

#endif
