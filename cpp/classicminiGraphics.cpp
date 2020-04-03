#include "classicminiGraphics.h"

namespace classicminigraphics {
	GLFWwindow* window;
	bool begin(int widthUsed, int heightUsed, const char* title) {
		if (!glfwInit()) { return false; }
		window = glfwCreateWindow(widthUsed, heightUsed, title, NULL, NULL);

		width = (float)widthUsed;
		height = (float)heightUsed;
		aspectDivider = width / height;

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

	float width = 0.0f;
	float height = 0.0f;
	float aspectDivider = 0.0f;

	float closeCamera = 0.1f;
	float farCamera = 100.0f;

	vec3 cameraPosition = vec3(0.0f);
}
