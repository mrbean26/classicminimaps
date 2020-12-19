#pragma once
#include "classicminiPathfinding.h"
#include "classicminimaps.h"

namespace general {
	float stringDistance(string positionOne, string positionTwo) {
		int colonIndex1 = positionOne.find(';');
		
		float x1 = stof(positionOne.substr(1, colonIndex1 - 1));
		float y1 = stof(positionOne.substr(colonIndex1 + 1, positionOne.find(']') - colonIndex1 - 1));

		int colonIndex2 = positionTwo.find(';');
		float x2 = stof(positionTwo.substr(1, colonIndex2 - 1));
		float y2 = stof(positionTwo.substr(colonIndex2 + 1, positionTwo.find(']') - colonIndex2 - 1));

		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}

	map<string, vector<string>> dictionaryFromConnectionFile(string connectionFilename) {
		vector<string> fileLines = savefiles::readLines(connectionFilename.data());
		map<string, vector<string>> returned;

		int lineCount = fileLines.size();
		for (int i = 0; i < lineCount; i++) {
			vector<string> currentLineData = savefiles::splitComma(fileLines[i]);
			string key = currentLineData[0];

			currentLineData.erase(currentLineData.begin());
			returned.insert(pair <string, vector<string>>(key, currentLineData));
		}

		return returned;
	}

	string findNearestPoint(string point, string connectionFileFilename) {
		vector<string> lines = savefiles::readLines(connectionFileFilename.data());
		int lineCount = lines.size();

		float lowestDistance = 100000000.0f;
		string currentNearest = "";

		for (int i = 0; i < lineCount; i++) {
			string currentLine = lines[i];
			string currentPoint = currentLine.substr(0, currentLine.find(','));

			float distanceCurrent = stringDistance(point, currentPoint);

			if (distanceCurrent < lowestDistance) {
				lowestDistance = distanceCurrent;
				currentNearest = currentPoint;
			}
		}

		return currentNearest;
	}
}

namespace specific {
	GLuint pathVAO = 0;
	GLuint pathVBO = 0;
	GLuint pathSize = 0;

	string findParentSquare(string point) {
		int colonIndex = point.find(';');

		float x = stof(point.substr(1, colonIndex - 1));
		float y = stof(point.substr(colonIndex + 1, point.find(']') - colonIndex - 1));

		map<string, vector<float>> mapSquareBoundaries = {{"HP", {450607.73, 467411.2, 1198673.0, 1216649.0}}, {"HT", {395777.0, 397487.26, 1137076.77, 1141236.34}}, {"HU", {417072.51, 468964.82, 1107878.88, 1202665.0}}, {"HY", {320187.87, 378691.61, 999504.0, 1056011.78}}, {"HZ", {419764.0, 422518.53, 1069800.0, 1074066.88}}, {"NA", {9123.0, 101854.0, 899133.0, 929405.0}}, {"NB", {99169.6, 200559.0, 899844.59, 966448.42}}, {"NC", {197965.14, 303922.0, 897548.0, 974728.35}}, {"ND", {298723.92, 352556.71, 913071.78, 1002503.4}}, {"NF", {9123.0, 101259.93000000001, 799621.35, 900127.54}}, {"NG", {99327.31, 203638.72, 795966.12, 900446.0}}, {"NH", {190571.71, 302032.34, 795662.4400000001, 901338.0}}, {"NJ", {298529.0, 401451.65, 797372.9500000001, 871270.88}}, {"NK", {398766.0, 413997.94, 824424.8200000001, 867419.0}}, {"NL", {62427.87, 101052.45, 738914.51, 800311.99}}, {"NM", {99546.1, 206354.76, 699453.25, 800452.4400000001}}, {"NN", {192147.79, 306104.0, 696679.88, 805219.42}}, {"NO", {295484.0, 394799.64, 698491.12, 801843.4400000001}}, {"NR", {116005.04000000001, 203077.0, 606993.0, 700781.88}}, {"NS", {194683.18, 303464.55, 597002.64, 703596.0}}, {"NT", {295488.07, 402211.9, 593212.28, 701998.93}}, {"NU", {397638.0, 428638.0, 596958.6, 654207.97}}, {"NW", {196379.62, 200549.15, 553785.0, 572634.87}}, {"NX", {198255.39, 301079.2, 505974.69, 602358.0}}, {"NY", {298441.76, 402638.63, 493242.93, 602803.9}}, {"NZ", {397807.21, 499865.25, 497219.29000000004, 601697.0}}, {"SD", {304630.13, 402026.83, 398947.60000000003, 504531.62}}, {"SE", {397733.0, 504586.56, 398779.05, 504818.51}}, {"SH", {213986.0, 301428.21, 298239.57, 394736.41000000003}}, {"SJ", {297679.0, 406874.23, 298018.0, 402250.26}}, {"SK", {396559.84, 502296.78, 284391.32, 404805.76}}, {"SM", {172414.55000000002, 201763.0, 198853.26, 241301.45}}, {"SN", {198206.88, 302068.0, 199083.27000000002, 302055.48}}, {"SO", {297466.0, 403171.05, 192269.94, 303401.72000000003}}, {"SP", {397289.72000000003, 502152.15, 197325.80000000002, 302654.72000000003}}, {"SR", {188368.0, 200387.0, 192700.75, 201024.0}}, {"SS", {198854.61000000002, 305484.93, 93609.05, 202991.78}}, {"ST", {296640.38, 409814.7, 93609.05, 206370.75}}, {"SU", {392217.79000000004, 501272.69, 98331.42, 203130.17}}, {"SV", {87441.91, 93482.77, 8045.93, 16236.44}}, {"SW", {134247.8, 201180.47, 11457.03, 81106.88}}, {"SX", {197339.72, 305707.55, 35346.47, 107352.89}}, {"SY", {299166.77, 401562.92, 68413.74, 101282.22}}, {"SZ", {398639.75, 498059.0, 75413.24, 102867.5}}, {"TA", {496118.85000000003, 542118.83, 397653.17, 498673.46}}, {"TF", {497443.68, 600920.43, 298664.5, 401415.95}}, {"TG", {598544.0, 653476.56, 298000.23, 345212.25}}, {"TL", {496212.78, 603065.04, 195515.89, 302682.14}}, {"TM", {597923.61, 655563.0, 199514.67, 304409.01}}, {"TQ", {492529.86, 601786.33, 99033.84, 203153.48}}, {"TR", {586009.34, 640145.72, 116618.62, 201743.44}}, {"TV", {546612.09, 562497.0, 95422.49, 101416.40000000001}} };
		for (const auto& pair : mapSquareBoundaries) {
			if (x > pair.second[0] && x < pair.second[1]) {
				if (y > pair.second[2] && y < pair.second[3]) {
					return pair.first;
				}
			}
		}

		return "Error";
	}

	string getSquareAverage(string squareCode) {
		map<string, string> squareAverages = { {"HP", "[460133.0635967811;1207590.4879176505]"}, {"HT", "[396712.90489051107;1138862.2727007293]"}, {"HU", "[440349.7243102664;1153617.544099596]"}, {"HY", "[341555.08685701306;1018427.9688231196]"}, {"HZ", "[421107.95305439323;1071696.9364853557]"}, {"NA", "[85239.82158823527;917245.0379999997]"}, {"NB", "[133954.5868961304;932630.2306692542]"}, {"NC", "[256012.2555232331;929031.3551837583]"}, {"ND", "[323065.76812718133;959495.3326003774]"}, {"NF", "[78842.15367874097;846545.5260965951]"}, {"NG", "[160220.55117444508;846249.5647590279]"}, {"NH", "[261301.36104478076;847352.2813044911]"}, {"NJ", "[361353.57889702497;833767.8680006693]"}, {"NK", "[407339.50749709265;847183.9497922226]"}, {"NL", "[84188.45081450335;766912.2522858657]"}, {"NM", "[175478.71311475302;741703.9258203191]"}, {"NN", "[252445.78940704203;740573.7117655098]"}, {"NO", "[341364.6304635833;742781.339348726]"}, {"NR", "[174517.37772740299;662267.2188091581]"}, {"NS", "[258594.625207643;660099.6323780724]"}, {"NT", "[335062.2913739599;660510.6671995752]"}, {"NU", "[414211.13662539853;618421.405637139]"}, {"NW", "[198977.54476251605;562438.9299422331]"}, {"NX", "[259731.9917291518;567709.0184396523]"}, {"NY", "[344283.72659500147;548955.2628937265]"}, {"NZ", "[433333.9356685883;544998.4208742521]"}, {"SD", "[363644.2051886873;431077.9595007093]"}, {"SE", "[438059.06643103505;435705.88809099793]"}, {"SH", "[264523.6528950351;353145.90601543145]"}, {"SJ", "[359568.7464700538;363871.4191616766]"}, {"SK", "[446430.84202762967;349141.9809713457]"}, {"SM", "[191596.82616847733;216488.34053723648]"}, {"SN", "[255674.60763125334;236735.08348489855]"}, {"SO", "[362572.23552142084;250232.4515729941]"}, {"SP", "[445335.56989575806;259155.37377453217]"}, {"SR", "[195401.43307530455;197343.33557846042]"}, {"SS", "[269265.33558187535;155370.7884803975]"}, {"ST", "[346656.72908485204;159666.57792923474]"}, {"SU", "[458699.42360462557;149246.82489001864]"}, {"SV", "[90270.74016574578;12516.82119337017]"}, {"SW", "[174151.86104750793;43757.75509617316]"}, {"SX", "[256497.4313953831;69126.91315572029]"}, {"SY", "[350036.1524294349;89875.61771326113]"}, {"SZ", "[433673.8668124431;92937.82958701794]"}, {"TA", "[513054.3123009241;436778.669937245]"}, {"TF", "[540769.6017285397;338270.5513178687]"}, {"TG", "[625577.4193871586;316507.2078721465]"}, {"TL", "[543066.458311219;240440.49110250155]"}, {"TM", "[622160.5387254267;258588.7515308007]"}, {"TQ", "[539478.9954586509;163616.24599880082]"}, {"TR", "[619521.8172848878;152139.40527704774]"}, {"TV", "[555955.4136257824;99019.96002188436]"} };
		return squareAverages[squareCode];
	}

	void findAndLoadPath(string startPosition, string endPosition) {
		vector<vec2> path = aStar::returnFullPath(startPosition, endPosition);
		cout << path.size() << endl;
		glGenVertexArrays(1, &pathVAO);
		glBindVertexArray(pathVAO);

		glGenBuffers(1, &pathVBO);
		glBindBuffer(GL_ARRAY_BUFFER, pathVBO);

		vector<float> finalVertices;
		int pointCount = path.size();

		for (int v = 0; v < pointCount - 1; v++) {
			// initial road line
			vec2 first = path[v] / classicminimaps::scaleDivider;

			finalVertices.push_back(first.x);
			finalVertices.push_back(first.y);

			int nextIndex = v + 1;
			vec2 second = path[nextIndex] / classicminimaps::scaleDivider;

			finalVertices.push_back(second.x);
			finalVertices.push_back(second.y);

			// arrow
			vec2 middlePoint = vec2((first.x + second.x) / 2.0f, (first.y + second.y) / 2.0f);
			float bearing = gridMath::bearing(first, second);
			float lineLength = glm::distance(first, second);

			// arrow line one
			finalVertices.push_back(middlePoint.x);
			finalVertices.push_back(middlePoint.y);

			finalVertices.push_back(middlePoint.x + sin(radians(bearing + classicminimaps::arrowAngle)) * lineLength * 0.15f);
			finalVertices.push_back(middlePoint.y + cos(radians(bearing + classicminimaps::arrowAngle)) * lineLength * 0.15f);

			// arrow line two
			finalVertices.push_back(middlePoint.x);
			finalVertices.push_back(middlePoint.y);

			finalVertices.push_back(middlePoint.x + sin(radians(bearing - classicminimaps::arrowAngle)) * lineLength * 0.15f);
			finalVertices.push_back(middlePoint.y + cos(radians(bearing - classicminimaps::arrowAngle)) * lineLength * 0.15f);
		}

		glBufferData(GL_ARRAY_BUFFER, finalVertices.size() * sizeof(float), finalVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		pathSize = (GLuint)(finalVertices.size() / 2);
	}

	void drawPath() {
		glLineWidth(2.0f);

		int shaderProgram = classicminimaps::shaderProgram;
		glUseProgram(shaderProgram);
		shader::setVectorFour(shaderProgram, "lineColours", vec4(1.0f, 0.0f, 0.0f, 1.0f));
		shader::setMat4(shaderProgram, "view", classicminigraphics::viewMatrix());
		shader::setMat4(shaderProgram, "projection", classicminigraphics::projectionMatrix());

		glBindVertexArray(pathVAO);
		glDrawArrays(GL_LINES, 0, pathSize);
	}
}

namespace aStar {
	class Node {
	public:
		int parent = -1;
		string position;
		vec2 floatPosition;

		float g;
		float h;
		float f;

		Node(int nodeParent, string stringPosition) {
			parent = nodeParent;
			position = stringPosition;

			g = 0.0f;
			h = 0.0f;
			f = 0.0f;
			
			int colonIndex = position.find(';');
			float x = stof(position.substr(1, colonIndex - 1));
			float y = stof(position.substr(colonIndex + 1, position.find(']') - colonIndex - 1));
			
			floatPosition = vec2(x, y);
		}

		bool operator == (const Node& Ref) {
			return position == Ref.position;
		}
	};

	vector<vec2> aStarFloat(string startPosition, string endPosition, map < string, vector<string>> connectionDictionary) {
		Node startNode = Node(-1, startPosition);
		Node endNode = Node(-1, endPosition);

		vector<Node> openList = { startNode };
		vector<Node> closedList = {};
		
		while (openList.size() > 0) {
			Node currentNode = openList[0];
			int currentIndex = 0;

			int openListSize = openList.size();
			for (int i = 0; i < openListSize; i++) {
				if (openList[i].f < currentNode.f) {
					currentNode = openList[i];
					currentIndex = i;
				}
			}

			openList.erase(openList.begin() + currentIndex);
			closedList.push_back(currentNode);
			int closedListLength = closedList.size();
			
			if (currentNode == endNode) {
				vector<vec2> path = { currentNode.floatPosition };

				while (!(currentNode == startNode)) { // current != nullptr
					path.push_back(currentNode.floatPosition);
					currentNode = closedList[currentNode.parent];
				}

				return path;
			}

			vector<string> childrenPositions = connectionDictionary[currentNode.position];
			vector<Node> children = {};
			
			int childrenPositionsCount = childrenPositions.size();
			for (int i = 0; i < childrenPositionsCount; i++) {
				children.push_back(Node(closedListLength - 1, childrenPositions[i]));
			}

			int childCount = children.size();
			for (int i = 0; i < childCount; i++) {
				if (find(closedList.begin(), closedList.end(), children[i]) != closedList.end()) {
					continue;
				}

				children[i].g = currentNode.g + 1.0f;
				children[i].h = pow(children[i].floatPosition.x - endNode.floatPosition.x, 2) + pow(children[i].floatPosition.y - endNode.floatPosition.y, 2);
				children[i].f = children[i].g + children[i].h;

				bool skip = false;
				int openListCount = openList.size();

				for (int a = 0; a < openListCount; a++) {
					if (children[i] == openList[a] && children[i].g > openList[a].g) {
						skip = true;
						break;
					}
				}

				if (skip) {
					continue;
				}

				openList.push_back(children[i]);
			}
		}
	}

	vector<string> aStarString(string startPosition, string endPosition, map < string, vector<string>> connectionDictionary) {
		Node startNode = Node(-1, startPosition);
		Node endNode = Node(-1, endPosition);

		vector<Node> openList = { startNode };
		vector<Node> closedList = {};

		while (openList.size() > 0) {
			Node currentNode = openList[0];
			int currentIndex = 0;

			int openListSize = openList.size();
			for (int i = 0; i < openListSize; i++) {
				if (openList[i].f < currentNode.f) {
					currentNode = openList[i];
					currentIndex = i;
				}
			}

			openList.erase(openList.begin() + currentIndex);
			closedList.push_back(currentNode);
			int closedListLength = closedList.size();

			if (currentNode == endNode) {
				vector<string> path = { currentNode.position };

				while (!(currentNode == startNode)) { // current != nullptr
					path.push_back(currentNode.position);
					currentNode = closedList[currentNode.parent];
				}

				return path;
			}

			vector<string> childrenPositions = connectionDictionary[currentNode.position];
			vector<Node> children = {};

			int childrenPositionsCount = childrenPositions.size();
			for (int i = 0; i < childrenPositionsCount; i++) {
				children.push_back(Node(closedListLength - 1, childrenPositions[i]));
			}

			int childCount = children.size();
			for (int i = 0; i < childCount; i++) {
				if (find(closedList.begin(), closedList.end(), children[i]) != closedList.end()) {
					continue;
				}

				children[i].g = currentNode.g + 1.0f;
				children[i].h = pow(children[i].floatPosition.x - endNode.floatPosition.x, 2) + pow(children[i].floatPosition.y - endNode.floatPosition.y, 2);
				children[i].f = children[i].g + children[i].h;

				bool skip = false;
				int openListCount = openList.size();

				for (int a = 0; a < openListCount; a++) {
					if (children[i] == openList[a] && children[i].g > openList[a].g) {
						skip = true;
						break;
					}
				}

				if (skip) {
					continue;
				}
				cout << children[i].parent << endl;
				openList.push_back(children[i]);
			}
		}
	}

	vector<vector<string>> createLargerSquarePath(string startPosition, string endPosition) {
		string startSquare = specific::findParentSquare(startPosition);
		string endSquare = specific::findParentSquare(endPosition);

		string startAverage = specific::getSquareAverage(startSquare);
		string endAverage = specific::getSquareAverage(endSquare);

		if (startSquare == endSquare) {
			return { {startSquare, startPosition, endPosition} };
		}
		
		map<string, vector<string>> mapSquareConnectionDictionary = general::dictionaryFromConnectionFile("assets/mapSquarePointConnections.txt");
		vector<string> squarePath = aStarString(startAverage, endAverage, mapSquareConnectionDictionary);

		squarePath[0] = startPosition;
		squarePath[squarePath.size() - 1] = endPosition;

		vector<vector<string>> returnedPaths = {};
		int squarePathCount = squarePath.size();
		
		for (int i = 0; i < squarePathCount; i++) {
			string currentPoint = squarePath[i];

			if (i == 0) {
				vector<string> newMiniPath = { startSquare };
				newMiniPath.push_back(startPosition);
				newMiniPath.push_back(general::findNearestPoint(squarePath[i + 1], "assets/mapData/connections/" + startSquare + "_RoadLink.shp.txt"));

				returnedPaths.push_back(newMiniPath);
			}
			if (i > 0 && i < squarePathCount - 1) {
				vector<string> newMiniPath = { specific::findParentSquare(squarePath[i]) };
				newMiniPath.push_back(general::findNearestPoint(returnedPaths[returnedPaths.size() - 1][2], "assets/mapData/connections/" + newMiniPath[0] + "_RoadLink.shp.txt"));
				newMiniPath.push_back(general::findNearestPoint(squarePath[i + 1], "assets/mapData/connections/" + newMiniPath[0] + "_RoadLink.shp.txt"));

				returnedPaths.push_back(newMiniPath);
			}
			if (i == squarePathCount - 1) {
				vector<string> newMiniPath = { specific::findParentSquare(squarePath[i]) };
				newMiniPath.push_back(general::findNearestPoint(returnedPaths[returnedPaths.size() - 1][2], "assets/mapData/connections/" + newMiniPath[0] + "_RoadLink.shp.txt"));
				newMiniPath.push_back(endPosition);

				returnedPaths.push_back(newMiniPath);
			}
		}

		return returnedPaths;
	}

	vector<vec2> returnFullPath(string startPosition, string endPosition) {
		vector<vector<string>> miniPaths = createLargerSquarePath(startPosition, endPosition);
		cout << "square done" << endl;
		vector<vec2> returnedPath = {};

		int miniPathCount = miniPaths.size();
		for (int i = 0; i < miniPathCount; i++) {
			map<string, vector<string>> connectionDictionary = general::dictionaryFromConnectionFile("assets/mapData/connections/" + miniPaths[i][0] + "_RoadLink.shp.txt");
			vector<vec2> currentPath = aStarFloat(miniPaths[i][1], miniPaths[i][2], connectionDictionary);

			returnedPath.insert(returnedPath.end(), currentPath.begin(), currentPath.end());
		}

		return returnedPath;
	}
}