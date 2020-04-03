#include "classicminimaps.h"
#include "functions.h"
#include "classicminiGraphics.h"

#include <vector>
#include <vec2.hpp>
#include <string>

#include <sstream>
#include <fstream>
#include <iostream>

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class mapSquare {
public:
	bool active = false;
	string filePath = "";
	vector<vector<vec2>> shapes;

	vec2 minPoints = vec2(0.0f);
	vec2 maxPoints = vec2(0.0f);

	void load(string fileName) {
		filePath = "assets/mapData/" + fileName;

		// read line by line
		string line;
		ifstream inFile(filePath);

		while (getline(inFile, line)) {
			// split between commas
			vector<vec2> thisShape;

			stringstream stringStream(line);
			int index = 0;
			while (stringStream.good()) {
				string subString;
				getline(stringStream, subString, ',');

				if (index % 2 == 0) {
					vec2 newVector = vec2(0.0f);
					thisShape.push_back(newVector);
				}

				int selectedIndex = (int)floor(index / 2);
				thisShape[selectedIndex][index % 2] = stof(subString);

				index = index + 1;
			}
			shapes.push_back(thisShape);
		}
	}

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint size = 0;

	void loadOpenGLAttributes() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		vector<float> vertices;
		int shapeCount = (int)shapes.size();

		for (int s = 0; s < shapeCount; s++) {
			int vertexCount = (int)shapes[s].size();

			for (int v = 0; v < vertexCount - 1; v++) {
				vertices.push_back(shapes[s][v].x / classicminimaps::scaleDivider);
				vertices.push_back(shapes[s][v].y / classicminimaps::scaleDivider);

				int nextIndex = v + 1;
				vertices.push_back(shapes[s][nextIndex].x / classicminimaps::scaleDivider);
				vertices.push_back(shapes[s][nextIndex].y / classicminimaps::scaleDivider);
			}
		}


		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		size = (GLuint)(vertices.size() / 2);
		active = true;
	}
};

namespace classicminimaps {
	int shaderProgram = 0;

	float scaleDivider = 100.0f;
	float height = 99.0f;

	float distanceToLoad = 0.0f;
	float rotationSearchInterval = 20.0f;

	mapSquare rivers;
	vector<mapSquare> currentMapSquares;

	void begin() {
		int vertex = createShader("assets/shaders/mapVertex.txt", GL_VERTEX_SHADER);
		int fragment = createShader("assets/shaders/mapFragment.txt", GL_FRAGMENT_SHADER);
		shaderProgram = createProgram({ vertex, fragment });

		rivers = mapSquare();
		rivers.load("WatercourseLink.shp.txt");
		rivers.loadOpenGLAttributes();

		// initialse mapsquares from file but don't load
		vector<string> allSquareValues = readLines("assets/textPointsMinMax.txt");
		int lineCount = allSquareValues.size();

		for (int l = 0; l < lineCount; l++) {
			vector<string> lineData = splitComma(allSquareValues[l]);

			mapSquare newSquare = mapSquare();
			newSquare.filePath = lineData[0] + "_RoadLink.shp.txt";
			newSquare.minPoints = vec2(stof(lineData[1]), stof(lineData[2]));
			newSquare.maxPoints = vec2(stof(lineData[3]), stof(lineData[4]));

			currentMapSquares.push_back(newSquare);
		}
	}

	void mainloop() {
		vec2 pos = vec2(519930.0f, 148090.0f);
		classicminigraphics::cameraPosition = vec3(pos.x / classicminimaps::scaleDivider, pos.y / classicminimaps::scaleDivider, height);

		loadChunks();
		render();
	}

	void loadChunks() {
		int squareCount = currentMapSquares.size();

		for (int i = 0; i < squareCount; i++) {
			if (currentMapSquares[i].active) {
				continue;
			}

			for (float r = 0.0f; r < 360.0f; r += rotationSearchInterval) {
				float xAdded = cos(radians(r)) * distanceToLoad;
				float yAdded = sin(radians(r)) * distanceToLoad;
				vec2 searchPos = vec2(xAdded, yAdded) + vec2(classicminigraphics::cameraPosition);

				if (searchPos.x >= currentMapSquares[i].minPoints.x / scaleDivider && searchPos.x <= currentMapSquares[i].maxPoints.x / scaleDivider) {
					if (searchPos.y >= currentMapSquares[i].minPoints.y / scaleDivider && searchPos.y <= currentMapSquares[i].maxPoints.y / scaleDivider) {
						currentMapSquares[i].load(currentMapSquares[i].filePath);
						currentMapSquares[i].loadOpenGLAttributes();

						break;
					}
				}
			}
		}
	}

	void render() {
		glLineWidth(1.0f);

		setVectorFour(shaderProgram, "lineColours", vec4(1.0f));
		setMat4(shaderProgram, "view", translate(mat4(1.0f), -classicminigraphics::cameraPosition));
		setMat4(shaderProgram, "projection", perspective(radians(45.0f), classicminigraphics::aspectDivider,
			classicminigraphics::closeCamera, classicminigraphics::farCamera));

		glUseProgram(shaderProgram);
		int squareCount = (int)currentMapSquares.size();

		for (int i = 0; i < squareCount; i++) {
			if (!currentMapSquares[i].active) {
				continue;
			}

			glBindVertexArray(currentMapSquares[i].VAO);
			glDrawArrays(GL_LINES, 0, currentMapSquares[i].size);
		}

		setVectorFour(shaderProgram, "lineColours", vec4(0.0f, 0.0f, 1.0f, 1.0f));
		glBindVertexArray(rivers.VAO);
		glDrawArrays(GL_LINES, 0, rivers.size);
	}
}
