#include "classicminiGraphics.h"

namespace classicminigraphics {
	GLFWwindow* window;
	bool begin(int width, int height, const char* title) {
		if (!glfwInit()) { return false; }
		window = glfwCreateWindow(width, height, title, NULL, NULL);

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

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;
	}
}
