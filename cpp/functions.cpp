#define STB_IMAGE_IMPLEMENTATION
#include "functions.h"

namespace savefiles {
	vector<string> readLines(const char* fileName) {
		vector<string> allLines;
		ifstream newFile(fileName);
		string currentLine;
		if (!newFile) {
			cout << "File could not be opened: " << fileName << endl;
		}
		while (getline(newFile, currentLine)) {
			allLines.push_back(currentLine);
		}
		return allLines;
	}

	vector<string> splitComma(string used) {
		vector<string> returned;
		stringstream stringStream(used);

		while (stringStream.good()) {
			string substring;
			getline(stringStream, substring, ',');
			returned.push_back(substring);
		}

		return returned;
	}
}

namespace shader {
	int createShader(const char* filePath, GLenum shaderType) {
		// get lines from file
		string allLines;
		vector<string> shaderLines = savefiles::readLines(filePath);
		for (string line : shaderLines) {
			allLines = allLines + line + "\n";
		}
		const char* shaderSource = allLines.data();
		// compile
		int newShader = glCreateShader(shaderType);
		glShaderSource(newShader, 1, &shaderSource, NULL);
		glCompileShader(newShader);
		int compileResult;
		char infoLog[512];
		glGetShaderiv(newShader, GL_COMPILE_STATUS, &compileResult);
		if (!compileResult) {
			glGetShaderInfoLog(newShader, 512, NULL, infoLog);
			cout << "Shader compilation error: " << infoLog << endl;
			return 0;
		}
		return newShader;
	}

	void setVectorFour(int shader, const char* vecName, vec4 usedVec) {
		int location = glGetUniformLocation(shader, vecName);
		glUniform4f(location, usedVec.x, usedVec.y, usedVec.z, usedVec.w);
	}

	void setMat4(int shader, const char* matrixName, mat4 usedMatrix) {
		int location = glGetUniformLocation(shader, matrixName);
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(usedMatrix));
	}

	void setInt(int shader, const char* intName, int usedInt) {
		int location = glGetUniformLocation(shader, intName);
		glUniform1i(location, usedInt);
	}

	int createProgram(vector<int> shaders) {
		int newProgram = glCreateProgram();
		int shaderCount = (int)shaders.size();
		for (int i = 0; i < shaderCount; i++) {
			glAttachShader(newProgram, shaders[i]);
		}
		glLinkProgram(newProgram);
		return newProgram;
	}
}

namespace gridMath {
	double tanSquared(double x) {
		return tan(x) * tan(x);
	}

	double reciprocalCos(double x) {
		return 1.0f / cos(x);
	}

	double sinSquared(double x) {
		return sin(x) * sin(x);
	}

	vec2 gridToLatLng(vec2 grid) {
		double OSGB_F0 = 0.9996012717;
		double N0 = -100000.0;
		double E0 = 400000.0;
		double phi0 = radians(49.0);
		double lambda0 = radians(-2.0);

		double a = 6377563.396;
		double b = 6356256.909;
		double eSquared = ((a*a) - (b*b)) / (a*a);
		double phi = 0.0;
		double lambda1 = 0.0;
		double E = grid.x;
		double N = grid.y;
		double n = (a - b) / (a + b);
		double M = 0.0;
		double phiPrime = ((N - N0) / (a * OSGB_F0)) + phi0;

		while ((N - N0 - M) >= 0.001) {
			double M = (b * OSGB_F0) * (((1 + n + ((5.0 / 4.0) * n * n) + ((5.0 / 4.0) * n * n * n)) * (phiPrime - phi0)) - (((3 * n) + (3 * n * n) + ((21.0 / 8.0) * n * n * n)) * sin(phiPrime - phi0) * cos(phiPrime + phi0)) + ((((15.0 / 8.0) * n * n) + ((15.0 / 8.0) * n * n * n)) * sin(2.0 * (phiPrime - phi0)) * cos(2.0 * (phiPrime + phi0))) - (((35.0 / 24.0) * n * n * n) * sin(3.0 * (phiPrime - phi0)) * cos(3.0 * (phiPrime + phi0))));
			phiPrime += (N - N0 - M) / (a * OSGB_F0);

			double v = a * OSGB_F0 * pow(1.0 - eSquared * sinSquared(phiPrime), -0.5);
			double rho = a * OSGB_F0 * (1.0 - eSquared) * pow(1.0 - eSquared * sinSquared(phiPrime), -1.5);
			double etaSquared = (v / rho) - 1.0;
			double VII = tan(phiPrime) / (2 * rho * v);
			double VIII = (tan(phiPrime) / (24.0 * rho * pow(v, 3.0))) * (5.0 + (3.0 * tanSquared(phiPrime)) + etaSquared - (9.0 * tanSquared(phiPrime) * etaSquared));
			double IX = (tan(phiPrime) / (720.0 * rho * pow(v, 5.0))) * (61.0 + (90.0 * tanSquared(phiPrime)) + (45.0 * tanSquared(phiPrime) * tanSquared(phiPrime)));
			double X = reciprocalCos(phiPrime) / v;
			double XI = (reciprocalCos(phiPrime) / (6.0 * v * v * v)) * ((v / rho) + (2 * tanSquared(phiPrime)));
			double XII = (reciprocalCos(phiPrime) / (120.0 * pow(v, 5.0))) * (5.0 + (28.0 * tanSquared(phiPrime)) + (24.0 * tanSquared(phiPrime) * tanSquared(phiPrime)));
			double XIIA = (reciprocalCos(phiPrime) / (5040.0 * pow(v, 7.0))) * (61.0 + (662.0 * tanSquared(phiPrime)) + (1320.0 * tanSquared(phiPrime) * tanSquared(phiPrime)) + (720.0 * tanSquared(phiPrime) * tanSquared(phiPrime) * tanSquared(phiPrime)));

			double phi = phiPrime - (VII * pow(E - E0, 2.0)) + (VIII * pow(E - E0, 4.0)) - (IX * pow(E - E0, 6.0));
		}

		return vec2(degrees(phi), degrees(lambda1));
	}

	vec2 latLngToGrid(vec2 latLng) {
		double OSGB_F0 = 0.9996012717;
		double N0 = -100000.0;
		double E0 = 400000.0;
		double phi0 = radians(49.0);
		double lambda0 = radians(-2.0);
		double a = 6377563.396;
		double b = 6356256.909;
		double eSquared = ((a*a) - (b*b)) / (a*a);
		double phi = radians(latLng[0]);
		double lambda1 = radians(latLng[1]);
		double E = 0.0;
		double N = 0.0;
		double n = (a - b) / (a + b);
		double v = a * OSGB_F0 * pow(1.0 - eSquared * sinSquared(phi), -0.5);
		double rho = a * OSGB_F0 * (1.0 - eSquared) * pow(1.0 - eSquared * sinSquared(phi), -1.5);
		double etaSquared = (v / rho) - 1.0;
		double M = (b * OSGB_F0) * (((1.0 + n + ((5.0 / 4.0) * n * n) + ((5.0 / 4.0) * n * n * n)) * (phi - phi0)) - (((3.0 * n) + (3.0 * n * n) + ((21.0 / 8.0) * n * n * n)) * sin(phi - phi0) * cos(phi + phi0)) + ((((15.0 / 8.0) * n * n) + ((15.0 / 8.0) * n * n * n)) * sin(2.0 * (phi - phi0)) * cos(2.0 * (phi + phi0))) - (((35.0 / 24.0) * n * n * n) * sin(3.0 * (phi - phi0)) * cos(3.0 * (phi + phi0))));
		double I = M + N0;
		double II = (v / 2.0) * sin(phi) * cos(phi);
		double III = (v / 24.0) * sin(phi) * pow(cos(phi), 3.0) * (5.0 - tanSquared(phi) + (9.0 * etaSquared));
		double IIIA = (v / 720.0) * sin(phi) * pow(cos(phi), 5.0) * (61.0 - (58.0 * tanSquared(phi)) + pow(tan(phi), 4.0));
		double IV = v * cos(phi);
		double V = (v / 6.0) * pow(cos(phi), 3.0) * ((v / rho) - tanSquared(phi));
		double VI = (v / 120.0) * pow(cos(phi), 5.0) * (5.0 - (18.0 * tanSquared(phi)) + (pow(tan(phi), 4.0)) + (14.0 * etaSquared) - (58 * tanSquared(phi) * etaSquared));
		N = I + (II * pow(lambda1 - lambda0, 2.0)) + (III * pow(lambda1 - lambda0, 4.0)) + (IIIA * pow(lambda1 - lambda0, 6.0));
		E = E0 + (IV * (lambda1 - lambda0)) + (V * pow(lambda1 - lambda0, 3.0)) + (VI * pow(lambda1 - lambda0, 5.0));

		return dvec2(E, N);
	}

	bool onScreen(mat4 projection, mat4 view, vec3 position){
		vec4 pClip = projection * view * vec4(position, 1.0f);
		return abs(pClip.x) < pClip.w &&
			abs(pClip.y) < pClip.w &&
			0 < pClip.z &&
			pClip.z < pClip.w;
	}
}

namespace text {
	map<GLchar, Character> fontCharacters;
	void getFont(const char* fontPath, int fontSize) {
		map<GLchar, Character> returned;

		FT_Library ftLibrary;
		if (FT_Init_FreeType(&ftLibrary)) {
			cout << "Couldn't Init Freetype" << endl;
			return;
		}
		FT_Face newFont;
		if (FT_New_Face(ftLibrary, fontPath, 0, &newFont)) {
			cout << "Couldn't load font: " << fontPath << endl;
			return;
		}
		FT_Set_Pixel_Sizes(newFont, 0, fontSize);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		for (GLubyte c = 0; c < 128; c++) { //load first 128 of ASCII
			if (FT_Load_Char(newFont, c, FT_LOAD_RENDER)) {
				cout << "Couldn't Load Character" << c << endl;
				continue;
			}
			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_RED,
				newFont->glyph->bitmap.width,
				newFont->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE,
				newFont->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				texture,
				glm::ivec2(newFont->glyph->bitmap.width, newFont->glyph->bitmap.rows),
				glm::ivec2(newFont->glyph->bitmap_left, newFont->glyph->bitmap_top),
				(GLuint)newFont->glyph->advance.x
			};
			returned.insert(std::pair<GLchar, Character>(c, character));
		}
		FT_Done_Face(newFont);
		FT_Done_FreeType(ftLibrary);
		fontCharacters = returned;
	}

	GLuint textVAO, textVBO;
	int textShader = 0;

	void begin(int fontSize) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		getFont("assets/fontComic.ttf", fontSize);
		glBindTexture(GL_TEXTURE_2D, 0);
		//ready vbo & vao
		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		// shader
		int vertex = shader::createShader("assets/shaders/textVertex.txt", GL_VERTEX_SHADER);
		int fragment = shader::createShader("assets/shaders/textFragment.txt", GL_FRAGMENT_SHADER);
		textShader = shader::createProgram({ vertex, fragment });
	}

	vec2 textMeasurements(string text, float size) {
		vec2 measurement = vec2(0.0f);
		string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character currentCharacter = fontCharacters[*c];

			measurement.x = measurement.x + (currentCharacter.advance >> 6)* size;
			measurement.y = max(measurement.y, currentCharacter.size.y * size);
		}

		return measurement;
	}

	void renderText(string text, vec3 position, float size, bool centered, vec4 textColour) {
		vec3 usedPosition = position;

		if (centered) {
			usedPosition.x = usedPosition.x - textMeasurements(text, size).x / 2.0f;
		}

		glBindVertexArray(textVAO);
		glUseProgram(textShader);
		shader::setMat4(textShader, "view", classicminigraphics::viewMatrix());
		shader::setMat4(textShader, "projection", classicminigraphics::projectionMatrix());

		glActiveTexture(GL_TEXTURE0);
		shader::setInt(textShader, "text", 0);
		shader::setVectorFour(textShader, "textColor", textColour);

		string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character currentCharacter = fontCharacters[*c];
			GLfloat xPos = usedPosition.x + currentCharacter.bearing.x * size;
			GLfloat yPos = usedPosition.y - (currentCharacter.size.y - currentCharacter.bearing.y) * size;

			GLfloat w = currentCharacter.size.x * size;
			GLfloat h = currentCharacter.size.y * size;

			// update vertices
			GLfloat vertices[6][4] = {
				{ xPos,     yPos + h,   0.0, 0.0 },
				{ xPos,     yPos,       0.0, 1.0 },
				{ xPos + w, yPos,       1.0, 1.0 },

				{ xPos,     yPos + h,   0.0, 0.0 },
				{ xPos + w, yPos,       1.0, 1.0 },
				{ xPos + w, yPos + h,   1.0, 0.0 }
			};

			glBindTexture(GL_TEXTURE_2D, currentCharacter.textureID);
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			usedPosition.x = usedPosition.x + (currentCharacter.advance >> 6)* size;
		}
	}
}

namespace texture {
	void begin(){
		stbi_set_flip_vertically_on_load(true);
	}

	texture loadTexture(const char* filePath) {
		texture newTexture;
		glGenTextures(1, &newTexture.textureId);
		newTexture.data = stbi_load(filePath, &newTexture.width, &newTexture.height, &newTexture.channels, 4);
		newTexture.name = filePath;

		if (!newTexture.data) {
			cout << "File cannot be found: " << filePath << endl;
			newTexture.data = stbi_load("assets/images/emptyTexture.png", &newTexture.width, &newTexture.height, &newTexture.channels, 4);
			newTexture.name = "assets/images/emptyTexture.png";
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, newTexture.textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, newTexture.width,
			newTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, newTexture.data);
		glGenerateMipmap(GL_TEXTURE_2D);

		return newTexture;
	}
	
	void enableTexture(texture usedTexture) {
		glBindTexture(GL_TEXTURE_2D, usedTexture.textureId);
	}
}
