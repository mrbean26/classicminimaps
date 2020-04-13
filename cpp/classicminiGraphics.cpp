#include "classicminiGraphics.h"

#include <gtc/matrix_transform.hpp>

namespace classicminigraphics {
	GLFWwindow* window;
	bool begin(int widthUsed, int heightUsed, const char* title, bool fullscreen) {
		if (!glfwInit()) { return false; }
		window = nullptr;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

		if (!fullscreen) {
			window = glfwCreateWindow(widthUsed, heightUsed, title, NULL, NULL);

			width = (float)widthUsed;
			height = (float)heightUsed;
			aspectDivider = width / height;
		}
		if (fullscreen) {
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* monitorMode = glfwGetVideoMode(primaryMonitor);

			width = monitorMode->width;
			height = monitorMode->height;

			window = glfwCreateWindow(width, height, title, primaryMonitor, NULL);
			aspectDivider = width / height;
		}

		if (!window) {
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK) {
			return false;
		}

		return true;
	}

	void endEvents() {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	void startEvents() {
		glClear(GL_COLOR_BUFFER_BIT);

		float currentTime = (float) glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
	}

	mat4 viewMatrix(){
		return translate(mat4(1.0f), -classicminigraphics::cameraPosition);
	}

	mat4 projectionMatrix(){
		return perspective(radians(45.0f), classicminigraphics::aspectDivider,
			classicminigraphics::closeCamera, classicminigraphics::farCamera);
	}

	float width = 0.0f;
	float height = 0.0f;
	float aspectDivider = 0.0f;

	float closeCamera = 0.1f;
	float farCamera = 1000.0f;

	vec3 cameraPosition = vec3(0.0f);
}
