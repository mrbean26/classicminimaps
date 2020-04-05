#include "mapInterface.h"

namespace mapInterface {
	bool interfaceOpen = false;

	string postcodeText = "Code";
	void updatePostcodeText(int key, int mode) {
		bool numbered = key >= GLFW_KEY_0 && key <= GLFW_KEY_9;
		bool lettered = key >= GLFW_KEY_A && key <= GLFW_KEY_Z;

		if (!interfaceOpen) {
			return;
		}

		if (key == GLFW_KEY_BACKSPACE && mode != GLFW_RELEASE) {
			if (!postcodeText.empty()) {
				postcodeText.pop_back();
			}
		}

		if (!numbered && !lettered) {
			return;
		}

		if (postcodeText == "Code") {
			postcodeText = "";
		}

		int size = (int)postcodeText.size();
		if (size == 6) {
			return;
		}
		// update text
		if (mode == GLFW_PRESS) {
			postcodeText = postcodeText + (char)key;
		}
	}

	void renderNonInterface(float indentMultiplier, float textSize, float gapMultiplier) {
		float usedSize = (classicminimaps::height / (1 / textSize)) / 10000.0f;

		vec3 position = vec3(vec2(classicminigraphics::cameraPosition), 0.0f);
		vec3 addition = vec3(0.0f, tan(radians(22.5f)) * classicminimaps::height, 0.0f);
		vec2 measuredText = text::textMeasurements("LatLong1234567890", usedSize);
		vec3 subtraction = vec3(0.0f, measuredText.y * indentMultiplier, 0.0f);

		text::renderText("Longitude: 52.23122312", position + addition - subtraction, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		text::renderText("Latitude: 52.23122312", position + addition - subtraction * gapMultiplier, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));

		text::renderText("Location: Brockham", position - addition + subtraction, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	void renderInterfaceItems(float textSize, float gapMultiplier) {
		float usedSize = (classicminimaps::height / (1 / textSize)) / 10000.0f;

		vec3 position = vec3(vec2(classicminigraphics::cameraPosition), 0.0f);
		vec2 measuredText = text::textMeasurements("LatLong1234567890", usedSize);
		vec3 addition = vec3(0.0f, (measuredText.y / 2.0f) * gapMultiplier, 0.0f);

		text::renderText("Enter Postcode", position + addition, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		text::renderText(postcodeText, position - addition, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	void mainloop() {
		renderNonInterface(1.25f, 1.0f, 2.25f);
		if (interfaceOpen) {
			renderInterfaceItems(1.5f, 1.25f);
		}
	}

	void openInterface() {
		interfaceOpen = true;
	}

	void closeInterface() {
		interfaceOpen = false;

		if (postcodeText.empty()) {
			postcodeText = "Code";
		}
	}
}

