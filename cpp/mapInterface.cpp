#include "mapInterface.h"
#include "classicminiPathfinding.h"

#include <thread>

namespace mapInterface {
	bool interfaceOpen = false;
	vec2 postcodesLocation = vec2(-1.0f);
	string currentPostcode = "";

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
			float x = stof(data[1]);
			float y = stof(data[2]);

			if (data[0] == postcode) {
				postcodesLocation.x = x;
				postcodesLocation.y = y;
				
				specific::finishedPathfind = false;
				specific::startedFinding = false;
				return "Found postcode!";
			}
		}

		postcodesLocation = vec2(-1.0f);
		return "Not found postcode.";
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
		//text::renderText("Distance: " + getRouteDistance(), position - addition + subtraction * gapMultiplier, usedSize, true, vec4(1.0f, 0.0f, 0.0f, 1.0f));
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

