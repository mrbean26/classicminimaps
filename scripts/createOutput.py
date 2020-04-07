from math import sqrt

def distance(pointOne, pointTwo):
	one = pointOne.replace("[", "").replace("]", "")
	two = pointTwo.replace("[", "").replace("]", "")

	oneData = one.split(";")
	twoData = two.split(";")

	x1 = float(oneData[0])
	y1 = float(oneData[1])

	x2 = float(twoData[0])
	y2 = float(twoData[1])

	return math.sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))

def onLineBetweenPoints(linePointOne, linePointTwo, checkPoint):
	distanceOne = distance(linePointOne, checkPoint)
	distanceTwo = distance(linePointTwo, checkPoint)
	distanceThree = distance(linePointOne, linePointTwo)

	if distanceOne + distanceTwo == distanceThree:
		return True
	return False

openedFile = open("HT_RoadLink.shp.txt", "r")
lines = openedFile.readlines()
openedFile.close()

dictionary = {}

for line in lines:
	currentLine = line.strip()
	data = currentLine.split(",")

	itemCount = len(data)
	for i in range(itemCount - 2):
		if i % 2 == 1:
			continue

		currentKey = "[" + data[i] + ";" + data[i + 1] + "]"
		if currentKey not in dictionary:
			dictionary.update({ currentKey : []})
		if i > 1:
			newValueOne = "[" + data[i - 2] + ";" + data[i - 1] + "]"
			newValueTwo = "[" + data[i + 2] + ";" + data[i + 3] + "]"

			dictionary[currentKey].append(newValueOne)
			dictionary[currentKey].append(newValueTwo)
		if i == 0:
			currentValue = "[" + data[i + 2] + ";" + data[i + 3] + "]"
			dictionary[currentKey].append(currentValue)

allKeys = dictionary.keys()
for key in allKeys:
	outputFile = open("output.txt", "a")

	outputLine = key + ","
	for item in dictionary[key]:
		outputLine = outputLine + item + ","
	outputLine = outputLine[:-1] + "\n"

	outputFile.write(outputLine)
	outputFile.close()
