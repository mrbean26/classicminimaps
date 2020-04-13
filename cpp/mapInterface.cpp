#include "mapInterface.h"

namespace mapInterface {
	bool interfaceOpen = false;
	vec2 postcodesLocation = vec2(-1.0f);
	string currentPostcode = "";
	GLuint postcodeVAO, postcodeVBO;

	string getRouteDistance() {
		if (postcodesLocation == vec2(-1.0f)) {
			return "No Route Set.";
		}

		float kmDistance = distance(classicminimaps::location, postcodesLocation) / 1000.0f;
		return to_string(kmDistance / 1.609f) + " miles";
	}

	string searchForPostcode(string postcode) {
		int length = (int)postcode.length();
		if (length < 5 || length > 7) {
			return "Not correct postcode.";
		}
		
		string searchTerm = "";
		if (isalpha(postcode.at(0))) {
			searchTerm = searchTerm + postcode.at(0);
			if (isalpha(postcode.at(1))) {
				searchTerm = searchTerm + postcode.at(1);
			}
		}

		string newSearchTerm = "";
		int searchLength = (int)searchTerm.length();
		for (int i = 0; i < searchLength; i++) {
			int ascii = (int)searchTerm.at(i);
			newSearchTerm = newSearchTerm + (char) (ascii + 32);
		}
		newSearchTerm = "assets/postcodes/" + newSearchTerm + ".csv.txt";

		if (!savefiles::fileExists(newSearchTerm)) {
			return "Incorrect postcode.";
		}

		vector<string> allLines = savefiles::readLines(newSearchTerm.data());
		int lineCount = (int)allLines.size();

		for (int l = 0; l < lineCount; l++) {
			vector<string> data = savefiles::splitComma(allLines[l]);
			if (data[0] == postcode) {
				postcodesLocation.x = stof(data[1]);
				postcodesLocation.y = stof(data[2]);

				glBindVertexArray(postcodeVAO);
				glBindBuffer(GL_ARRAY_BUFFER, postcodeVBO);
				vector<float> vertices = { classicminigraphics::cameraPosition.x, classicminigraphics::cameraPosition.y, 
					postcodesLocation.x / classicminimaps::scaleDivider, postcodesLocation.y / classicminimaps::scaleDivider };
				
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

				return "Found postcode!";
			}
		}

		postcodesLocation = vec2(-1.0f);
		return "Not found postcode.";
	}

	void renderPostcodeLine() {
		glBindVertexArray(postcodeVAO);
		glUseProgram(classicminimaps::shaderProgram);
		glLineWidth(2.0f);

		shader::setVectorFour(classicminimaps::shaderProgram, "lineColours", vec4(0.8f, 1.0f, 0.0f, 1.0f));
		shader::setMat4(classicminimaps::shaderProgram, "view", classicminigraphics::viewMatrix());
		shader::setMat4(classicminimaps::shaderProgram, "projection", classicminigraphics::projectionMatrix());

		glDrawArrays(GL_LINES, 0, 2);
	}

	void beginPostcodeInfo() {
		glGenVertexArrays(1, &postcodeVAO);
		glBindVertexArray(postcodeVAO);

		glGenBuffers(1, &postcodeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, postcodeVBO);

		vector<float> vertices = { 0.0f, 0.0f, 0.0f, 0.0f };
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

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
		if (size == 7) {
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

		text::renderText("Longitude: " + to_string(classicminimaps::longLat.x), position + addition - subtraction, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		text::renderText("Latitude: " + to_string(classicminimaps::longLat.y), position + addition - subtraction * gapMultiplier, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));

		text::renderText("Location: " + classicminimaps::nearestLocation, position - addition + subtraction, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		text::renderText("Distance: " + getRouteDistance(), position - addition + subtraction * gapMultiplier, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	void renderInterfaceItems(float textSize, float gapMultiplier) {
		float usedSize = (classicminimaps::height / (1 / textSize)) / 10000.0f;

		vec3 position = vec3(vec2(classicminigraphics::cameraPosition), 0.0f);
		vec2 measuredText = text::textMeasurements("LatLong1234567890", usedSize);
		vec3 addition = vec3(0.0f, (measuredText.y / 2.0f) * gapMultiplier, 0.0f);

		text::renderText("Enter Postcode", position + addition, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
		text::renderText(postcodeText, position - addition, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	void begin() {
		beginPostcodeInfo();
	}

	void mainloop() {
		renderPostcodeLine();
		renderNonInterface(1.25f, 1.0f, 2.25f);
		if (interfaceOpen) {
			renderInterfaceItems(1.5f, 1.25f);
		}
	}

	void openInterface() {
		interfaceOpen = true;
	}

	void closeInterface() {
		if (!postcodeText.empty() && interfaceOpen) {
			if (postcodeText != currentPostcode) {
				cout << searchForPostcode(postcodeText) << endl;
				currentPostcode = postcodeText;
			}
		}
		interfaceOpen = false;

		if (postcodeText.empty()) {
			postcodeText = "Code";
		}
	}
}

