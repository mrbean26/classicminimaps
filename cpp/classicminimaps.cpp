#include "classicminimaps.h"
#include <stdlib.h>
namespace classicminimaps {
	int shaderProgram = 0;

	float scaleDivider = 100.0f;
	float height = 15.0f;
	float defaultHeight = 15.0f;

	float distanceToLoad = 100.0f;
	float rotationSearchInterval = 20.0f;

	vec4 textColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float textSize = 0.001f;

	mapSquare rivers;
	vector<mapSquare> currentMapSquares;
	vector<openName> toRender;

	vector<vec2> speedCameraLocations;
	GLuint speedVAO, speedVBO, speedSize;
	int speedShader = 0;
	texture::texture policeIconTexture;

	float speedCameraSize = 0.2f;
	float cameraUpSpeed = 10.0f;
	float cameraForwardSpeed = 4.0f;
	float xShift = 0.0f;
	float yShift = 0.0f;

	texture::texture yourIcon;
	float yourIconSize = 0.2f;

	void begin() {
		glClearColor(0.172f, 0.749f, 0.694f, 1.0f);

		// load
		int vertex = shader::createShader("assets/shaders/mapVertex.txt", GL_VERTEX_SHADER);
		int fragment = shader::createShader("assets/shaders/mapFragment.txt", GL_FRAGMENT_SHADER);
		shaderProgram = shader::createProgram({ vertex, fragment });

		rivers = mapSquare();
		rivers.load("WatercourseLink.shp.txt");
		rivers.loadOpenGLAttributes();

		// initialse mapsquares from file but don't load
		vector<string> allSquareValues = savefiles::readLines("assets/textPointsMinMax.txt");
		int lineCount = (int) allSquareValues.size();

		for (int l = 0; l < lineCount; l++) {
			vector<string> lineData = savefiles::splitComma(allSquareValues[l]);

			mapSquare newSquare = mapSquare();
			newSquare.justFileName = lineData[0] + "_RoadLink.shp.txt";
			newSquare.minPoints = vec2(stof(lineData[1]), stof(lineData[2]));
			newSquare.maxPoints = vec2(stof(lineData[3]), stof(lineData[4]));

			currentMapSquares.push_back(newSquare);
		}
		// speed
		loadSpeedCameras();
		glfwSetKeyCallback(classicminigraphics::window, keyPresses);
		glfwSetMouseButtonCallback(classicminigraphics::window, mousePressed);
		// your icon
		yourIcon = texture::loadTexture("assets/images/yourIcon.png");
	}

	vec2 location = vec2(0.0f);
	vec2 longLat = vec2(0.0f);
	void mainloop() {
		longLat = getLatLong();
		location = gridMath::latLngToGrid_WGS84(longLat);
		classicminigraphics::cameraPosition = vec3(location.x / classicminimaps::scaleDivider + xShift, location.y / classicminimaps::scaleDivider + yShift, height);

		loadChunks();
		loadTexts();
		render();
		renderMapTexts();
		drawSpeedCameras();
	}

	void mousePressed(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			mapInterface::openInterface();
		}
		if (button == GLFW_MOUSE_BUTTON_2) {
			mapInterface::closeInterface();
		}
	}

	void keyPresses(GLFWwindow* window, int key, int scancode, int action, int mods) {
		mapInterface::updatePostcodeText(key, action);
		if (key == GLFW_KEY_R) {
			if (!mapInterface::interfaceOpen) {
				xShift = 0.0f;
				yShift = 0.0f;
				height = defaultHeight;
			}
		}
		if (key == GLFW_KEY_W) {
			if (!mapInterface::interfaceOpen) {
				height = height + classicminigraphics::deltaTime * cameraUpSpeed;
			}
		}
		if (key == GLFW_KEY_S) {
			if (!mapInterface::interfaceOpen) {
				height = height - classicminigraphics::deltaTime * cameraUpSpeed;
			}
		}
		if (key == GLFW_KEY_UP) {
			if (!mapInterface::interfaceOpen) {
				yShift = yShift + classicminigraphics::deltaTime * cameraForwardSpeed * (height / defaultHeight);
			}
		}
		if (key == GLFW_KEY_DOWN) {
			if (!mapInterface::interfaceOpen) {
				yShift = yShift - classicminigraphics::deltaTime * cameraForwardSpeed * (height / defaultHeight);
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (!mapInterface::interfaceOpen) {
				xShift = xShift + classicminigraphics::deltaTime * cameraForwardSpeed * (height / defaultHeight);
			}
		}
		if (key == GLFW_KEY_LEFT) {
			if (!mapInterface::interfaceOpen) {
				xShift = xShift - classicminigraphics::deltaTime * cameraForwardSpeed * (height / defaultHeight);
			}
		}
	}

	void loadChunks() {
		int squareCount = (int)currentMapSquares.size();

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
						currentMapSquares[i].load(currentMapSquares[i].justFileName);
						currentMapSquares[i].loadOpenGLAttributes();
						currentMapSquares[i].loadOpenNames();

						break;
					}
				}
			}
		}
	}

	void render() {
		glLineWidth(1.0f);
		
		glUseProgram(shaderProgram);
		shader::setVectorFour(shaderProgram, "lineColours", vec4(1.0f));
		shader::setMat4(shaderProgram, "view", classicminigraphics::viewMatrix());
		shader::setMat4(shaderProgram, "projection", classicminigraphics::projectionMatrix());

		int squareCount = (int)currentMapSquares.size();

		for (int i = 0; i < squareCount; i++) {
			if (!currentMapSquares[i].active) {
				continue;
			}

			glBindVertexArray(currentMapSquares[i].VAO);
			glDrawArrays(GL_LINES, 0, currentMapSquares[i].size);
		}

		shader::setVectorFour(shaderProgram, "lineColours", vec4(0.0f, 0.0f, 1.0f, 1.0f));
		glBindVertexArray(rivers.VAO);
		glDrawArrays(GL_LINES, 0, rivers.size);
	}

	void renderMapTexts() {
		if (classicminimaps::height < 25.0f) {
			int toRenderCount = (int)toRender.size();
			for (int t = 0; t < toRenderCount; t++) {
				text::renderText(toRender[t].name, vec3(toRender[t].position.x / scaleDivider, toRender[t].position.y / scaleDivider, 0.0f),
					textSize * (height / defaultHeight), true, textColour);
			}
		}

		toRender.clear();
	}

	bool objectWillBeOnScreen(vec2 objectPosition) {
		// will not work with a rotated camera
		if (objectPosition.x <= classicminigraphics::cameraPosition.x + tan(radians(22.5f)) * height) {
			if (objectPosition.x >= classicminigraphics::cameraPosition.x - tan(radians(22.5f)) * height) {

				if (objectPosition.y <= classicminigraphics::cameraPosition.y + tan(radians(22.5f)) * height) {
					if (objectPosition.y >= classicminigraphics::cameraPosition.y - tan(radians(22.5f)) * height) {
						return true;
					}
				}
			}
		}
		return false;
	}

	void loadSpeedCameras(){
		vector<string> allLines = savefiles::readLines("assets/garminSpeedToText.txt");
		int lineCount = (int) allLines.size();

		for (int l = 0; l < lineCount; l++) {
			string currentLine = allLines[l];
			vector<string> data = savefiles::splitComma(currentLine);
			vec2 newVector = vec2(0.0f);

			newVector.x = stof(data[0]) / scaleDivider;
			newVector.y = stof(data[1]) / scaleDivider;

			speedCameraLocations.push_back(newVector);
		}
		// begin opengl
		vector<float> vertices = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f,

			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f, 1.0f
		};
		glGenVertexArrays(1, &speedVAO);
		glBindVertexArray(speedVAO);

		glGenBuffers(1, &speedVBO);
		glBindBuffer(GL_ARRAY_BUFFER, speedVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		speedSize = (GLuint) (vertices.size() / 4);
		// shaders
		int vertex = shader::createShader("assets/shaders/speedVertex.txt", GL_VERTEX_SHADER);
		int fragment = shader::createShader("assets/shaders/speedFragment.txt", GL_FRAGMENT_SHADER);
		speedShader = shader::createProgram({ vertex, fragment });
		// texture
		policeIconTexture = texture::loadTexture("assets/images/policeIcon.png");
	}

	void drawSpeedCameras(){
		glUseProgram(speedShader);
		glBindVertexArray(speedVAO);
		shader::setMat4(speedShader, "projection", classicminigraphics::projectionMatrix());
		shader::setMat4(speedShader, "view", classicminigraphics::viewMatrix());
		texture::enableTexture(policeIconTexture);

		int cameraCount = (int)speedCameraLocations.size();
		for (int i = 0; i < cameraCount; i++) {
			if (objectWillBeOnScreen(speedCameraLocations[i])) {
				mat4 modelMatrix = translate(mat4(1.0f), vec3(speedCameraLocations[i], 0.0f));
				shader::setMat4(speedShader, "model", scale(modelMatrix, vec3(speedCameraSize)));

				glDrawArrays(GL_TRIANGLES, 0, speedSize);
			}
		}

		// your icon
		vec2 usedCameraPosition = vec2(classicminigraphics::cameraPosition) - vec2(xShift, yShift);
		vec3 yourIconPosition = vec3(usedCameraPosition, 0.0f);
		float multiplier = (float)yourIcon.width / (float)yourIcon.height;
		vec3 yourIconScale = vec3(yourIconSize * multiplier, yourIconSize, 1.0f);
		yourIconScale = yourIconScale * vec3(height / defaultHeight);

		mat4 modelMatrix = translate(mat4(1.0f), yourIconPosition);
		modelMatrix = scale(modelMatrix, yourIconScale);
		shader::setMat4(speedShader, "model", modelMatrix);
		texture::enableTexture(yourIcon);

		glDrawArrays(GL_TRIANGLES, 0, speedSize);
	}

	bool runningThread = false;
	future<vec2> thread;
	vec2 loadLatLong() {
		if (!runningThread) {
			thread = async(getLatLong);
			runningThread = true;
			return longLat;
		}
		if (runningThread) {
			if (thread.wait_for(chrono::seconds(0)) == future_status::ready) {
				runningThread = false;
				return thread.get();
			}
			else {
				return longLat;
			}
		}
		return longLat;
	}

	vec2 getLatLong() {
		int osType = classicminiSystem::getOSType();

		if (osType == classicminiSystem::WINDOWS) {

		}

		if (osType == classicminiSystem::MAC_OSX) {
			string result = classicminiSystem::executeCommand("/Users/adamc/Desktop/whereami ; exit;");
			if (result == "") {
				return longLat;
			}
			vector<string> split = savefiles::splitNewLine(result);

			string first = "Latitude: ";
			string second = "Longitude: ";

			string latitude = split[0].substr(first.size(), split[0].size());
			string longitude = split[1].substr(second.size(), split[1].size());

			return vec2(stof(latitude), stof(longitude));
		}

		if (osType == classicminiSystem::LINUX) {

		}

		return longLat;
	}
	
	string nearestLocation = "";
	void loadTexts() {
		float lowestDistance = (float) INT_MAX;
		string currentLowest = "";

		int currentSquareCount = (int)currentMapSquares.size();

		for (int i = 0; i < currentSquareCount; i++) {
			int currentOpenNameCount = (int)currentMapSquares[i].allNames.size();

			for (int n = 0; n < currentOpenNameCount; n++) {
				vec2 checkPosition = vec2(currentMapSquares[i].allNames[n].position);
				checkPosition = checkPosition / vec2(scaleDivider);

				if (objectWillBeOnScreen(checkPosition)) {
					toRender.push_back(currentMapSquares[i].allNames[n]);
				}
				
				float newDistance = distance(location, currentMapSquares[i].allNames[n].position);
				if (newDistance < lowestDistance) {
					currentLowest = currentMapSquares[i].allNames[n].name;
					lowestDistance = newDistance;
				}
			}
		}

		nearestLocation = currentLowest;
	}
}

void mapSquare::load(string fileName){
	justFileName = fileName;
	fullFilePath = "assets/mapData/" + fileName;
	gridSquareName = justFileName.substr(0, 2);

	// read line by line
	vector<string> allLines = savefiles::readLines(fullFilePath.data());
	int lineCount = (int)allLines.size();

	for (int l = 0; l < lineCount; l++) {
		// split between commas
		vector<vec2> thisShape;
		vector<string> splitByComma = savefiles::splitComma(allLines[l]);
		int count = (int) splitByComma.size();

		for (int i = 0; i < count / 2; i++) {
			int multiplied = i * 2;
			vec2 newPoint = vec2(stof(splitByComma[multiplied]), stof(splitByComma[multiplied + 1]));
			thisShape.push_back(newPoint);
		}
		shapes.push_back(thisShape);
	}
}

void mapSquare::loadOpenGLAttributes(){
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

void mapSquare::loadOpenNames(){
	namePath = "assets/roadNames/" + gridSquareName + "Names.txt";
	vector<string> nameFileLines = savefiles::readLines(namePath.data());
	int lineCount = (int)nameFileLines.size();

	for (int l = 0; l < lineCount; l++) {
		vector<string> data = savefiles::splitComma(nameFileLines[l]);

		openName newNameReference = openName();
		newNameReference.name = data[0];

		newNameReference.position.x = stof(data[1]);
		newNameReference.position.y = stof(data[2]);

		allNames.push_back(newNameReference);
	}
}
