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

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))

def onLineBetweenPoints(linePointOne, linePointTwo, checkPoint):
	distanceOne = distance(linePointOne, checkPoint)
	distanceTwo = distance(linePointTwo, checkPoint)
	distanceThree = distance(linePointOne, linePointTwo)

	if distanceOne + distanceTwo == distanceThree:
		return True
	return False

openedFile = open("TQ_RoadLink.shp.txt", "r")
lines = openedFile.readlines()
openedFile.close()

dictionary = {}

def intersectsLineInDictionary(point):
	returned = []
	for key in dictionary.keys():
		if key == point:
			continue
		values = dictionary[key]

		valueCount = len(values)
		for v in range(valueCount):
			if onLineBetweenPoints(key, values[v], point) == True:
				returned.append([key, values[v]])

	return returned

lineCount = len(lines)
for l in range(lineCount):
	print("Starting line " + str(l + 1) + "/" + str(lineCount))
	currentLine = lines[l].strip()
	data = currentLine.split(",")

	itemCount = len(data)
	for i in range(itemCount - 1):
		if i % 2 == 1:
			continue

		currentKey = "[" + data[i] + ";" + data[i + 1] + "]"
		if currentKey not in dictionary:
			dictionary.update({ currentKey : []})

		newValues = []
		if i > 1 and i < itemCount - 2:
			newValueOne = "[" + data[i - 2] + ";" + data[i - 1] + "]"
			newValueTwo = "[" + data[i + 2] + ";" + data[i + 3] + "]"

			newValues = [newValueOne, newValueTwo]
		if i == 0:
			currentValue = "[" + data[i + 2] + ";" + data[i + 3] + "]"

			newValues = [currentValue]

		if i == itemCount - 2:
			currentValue = "[" + data[i - 2] + ";" + data[i - 1] + "]"

			newValues = [currentValue]

		for value in newValues:
			dictionary[currentKey].append(value)

# line intersections
keys = dictionary.keys()
keyCount = len(keys)

for k in range(keyCount):
	print("Starting key: " + str(k + 1) + "/" + str(keyCount))

	key = dictionary.keys()[k]
	intersections = intersectsLineInDictionary(key)

	if len(intersections) > 0:
		for line in intersections:
			dictionary[line[0]].remove(line[1])
			dictionary[line[1]].remove(line[0])

			dictionary[line[0]].append(key)
			dictionary[line[1]].append(key)

			dictionary[key].append(line[0])
			dictionary[key].append(line[1])


# write to file
allKeys = dictionary.keys()
for key in allKeys:
	outputFile = open("output.txt", "a")

	outputLine = key + ","
	for item in dictionary[key]:
		outputLine = outputLine + item + ","
	outputLine = outputLine[:-1] + "\n"

	outputFile.write(outputLine)
	outputFile.close()
