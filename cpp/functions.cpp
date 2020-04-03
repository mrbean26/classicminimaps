#include "functions.h"

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

int createShader(const char* filePath, GLenum shaderType) {
	// get lines from file
	string allLines;
	vector<string> shaderLines = readLines(filePath);
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

int createProgram(vector<int> shaders) {
	int newProgram = glCreateProgram();
	int shaderCount = (int) shaders.size();
	for (int i = 0; i < shaderCount; i++) {
		glAttachShader(newProgram, shaders[i]);
	}
	glLinkProgram(newProgram);
	return newProgram;
}

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
	double eSquared = (a - b) / a;
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
	double eSquared = (a - b) / a;
	double phi = radians(latLng[0]);
	double lambda1 = radians(latLng[1]);
	double E = 0.0;
	double N = 0.0;
	double n = (a - b) / (a + b);
	double v = a * OSGB_F0 * pow(1.0 - eSquared * sinSquared(phi), -0.5);
	double rho = a * OSGB_F0 * (1.0 - eSquared) * pow(1.0 - eSquared * sinSquared(phi), -1.5);
	double etaSquared = (v / rho) - 1.0;
	double M = (b * OSGB_F0) * (((1 + n + ((5.0 / 4.0) * n * n) + ((5.0 / 4.0) * n * n * n)) * (phi - phi0)) - (((3 * n) + (3 * n * n) + ((21.0 / 8.0) * n * n * n)) * sin(phi - phi0) * cos(phi + phi0)) + ((((15.0 / 8.0) * n * n) + ((15.0 / 8.0) * n * n * n)) * sin(2.0 * (phi - phi0)) * cos(2.0 * (phi + phi0))) - (((35.0 / 24.0) * n * n * n) * sin(3.0 * (phi - phi0)) * cos(3.0 * (phi + phi0))));
	double I = M + N0;
	double II = (v / 2.0) * sin(phi) * cos(phi);
	double III = (v / 24.0) * sin(phi) * pow(cos(phi), 3.0) * (5.0 - tanSquared(phi) + (9.0 * etaSquared));
	double IIIA = (v / 720.0) * sin(phi) * pow(cos(phi), 5.0) * (61.0 - (58.0 * tanSquared(phi)) + pow(tan(phi), 4.0));
	double IV = v * cos(phi);
	double V = (v / 6.0) * pow(cos(phi), 3.0) * ((v / rho) - tanSquared(phi));
	double VI = (v / 120.0) * pow(cos(phi), 5.0) * (5.0 - (18.0 * tanSquared(phi)) + (pow(tan(phi), 4.0)) + (14 * etaSquared) - (58 * tanSquared(phi) * etaSquared));
	N = I + (II * pow(lambda1 - lambda0, 2.0)) + (III * pow(lambda1 - lambda0, 4.0)) + (IIIA * pow(lambda1 - lambda0, 6.0));
	E = E0 + (IV * (lambda1 - lambda0)) + (V * pow(lambda1 - lambda0, 3.0)) + (VI * pow(lambda1 - lambda0, 5.0));

	return vec2(E, N);
}