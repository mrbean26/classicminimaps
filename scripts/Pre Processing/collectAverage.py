import glob

# create output file, USE ROAD POINT TEXT AS INPUT
outputFile = open("outputAverages.txt", "w+")
outputFile.close()

outputFile = open("outputAverages.txt", "a")
index = 0
for file in glob.glob("output/*Link.shp.txt"):

    print(index, 53)
    index += 1

    openedFile = open(file, 'r')
    allLines = openedFile.readlines()

    lineCount = len(allLines)
    xMin = 100000000000000
    xMax = 0

    yMin = 100000000000000
    yMax = 0

    yCount = 0
    xCount = 0

    xTotal = 0
    yTotal = 0
    for l in range(lineCount):
        currentLine = allLines[l].strip()

        allPointsString = currentLine.split(",")
        pointCount = len(allPointsString)

        for p in range(pointCount):
            point = float(allPointsString[p])
            if p % 2 == 0: #x
                xCount +=1
                xTotal += point
                xMin = min(xMin, point)
                xMax = max(xMax, point)
            if p % 2 == 1:
                yCount += 1
                yTotal+= point
                yMin = min(yMin, point)
                yMax = max(yMax, point)
    xAvg = xTotal / xCount
    yAvg = yTotal / yCount

    outputLine = file.replace("_RoadLink.shp.txt", "").replace("output\\", "") + "," + str(xAvg) + "," + str(yAvg)
    outputLine += "\n"

    outputFile.write(outputLine)
outputFile.close()
