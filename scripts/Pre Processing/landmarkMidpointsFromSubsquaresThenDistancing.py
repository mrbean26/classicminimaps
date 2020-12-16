import glob
allFiles = glob.glob("outputConnectionsQuarterSquares/*.shp.txt")
fileCount = len(allFiles)

def distanceStringAndFloat(pointOne, xPoint, yPoint):
	one = pointOne.replace("[", "").replace("]", "")
	oneData = one.split(";")

	x1 = float(oneData[0])
	y1 = float(oneData[1])

	return math.sqrt(pow(xPoint - x1, 2) + pow(yPoint - y1, 2))

for fileNumber in range(fileCount):
    # open file
    fileName = allFiles[fileNumber]
    openedFile = open(fileName)

    lines = openedFile.readlines()
    openedFile.close()

    if len(lines) == 0:
        continue

    # determine actual mid x and mid y
    xMinimum = float(lines[0].strip().split(",")[0].split(";")[0].replace("[", ""))
    xMaxmimum = float(lines[len(lines) - 1].strip().split(",")[0].split(";")[0].replace("[", ""))

    yMinimum = 9999999999
    yMaximum = 0

    for line in lines:
        currentLine = line.strip()
        yPosition = float(currentLine.split(",")[0].split(";")[1].replace("]", ""))

        yMinimum = min(yPosition, yMinimum)
        yMaximum = max(yPosition, yMaximum)

    accurateMidpointX = (xMinimum + xMaxmimum) / 2
    accurateMidpointY = (yMinimum + yMaximum) / 2

    # determine nearest node in subsquare to actual x and y, to use as landmark
    lowestDistanceCurrent = distanceStringAndFloat(lines[0].strip().split(",")[0], accurateMidpointX, accurateMidpointY)
    nearestNodeCurrentIndex = 0

    # loop over every point, start at 1 as already done 0
    for lineNumber in range(1, len(lines)):
        currentLine = lines[lineNumber].strip()
        point = currentLine.split(",")[0]

        currentDistance = distanceStringAndFloat(point, accurateMidpointX, accurateMidpointY)
        if currentDistance < lowestDistanceCurrent:
            lowestDistanceCurrent = currentDistance
            nearestNodeCurrentIndex = lineNumber

    landmarkNode = lines[nearestNodeCurrentIndex].strip().split(",")[0]

    # calculate distance from landmark to all other nodes in subsquare for use in ALT* pathfinding



    # write to file, in order of their occurence of subSquare file
    # for example TQ takes one line in output file, with landmark at start, then distances following



    # end

    print(fileNumber + 1, "/", "53")
