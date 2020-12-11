import glob
allFiles = glob.glob("outputConnections/*.shp.txt")
fileCount = len(allFiles)

for i in range(fileCount):
    fileName = allFiles[i]

    openedFile = open(fileName, "r")
    lines = openedFile.readlines()
    openedFile.close()

    xMinimum = float(lines[0].strip().split(",")[0].split(";")[0].replace("[", ""))
    xMaxmimum = float(lines[len(lines) - 1].strip().split(",")[0].split(";")[0].replace("[", ""))
    yMinimum = 9999999999
    yMaximum = 0

    for line in lines:
        currentLine = line.strip()
        yPosition = float(currentLine.split(",")[0].split(";")[1].replace("]", ""))

        yMinimum = min(yPosition, yMinimum)
        yMaximum = max(yPosition, yMaximum)

    xMiddle = (xMaxmimum + xMinimum) / 2
    yMiddle = (yMaximum + yMinimum) / 2

    topLeftLines = []
    topLeftFilename = fileName.replace("outputConnections", "outputConnectionsQuarterSquares").replace("Link", "Link_1")

    topRightLines = []
    topRightFilename = fileName.replace("outputConnections", "outputConnectionsQuarterSquares").replace("Link", "Link_2")

    bottomLeftLines = []
    bottomLeftFilename = fileName.replace("outputConnections", "outputConnectionsQuarterSquares").replace("Link", "Link_3")

    bottomRightLines = []
    bottomRightFilename = fileName.replace("outputConnections", "outputConnectionsQuarterSquares").replace("Link", "Link_4")

    for line in lines:
        currentLine = line.strip()
        xPosition = float(currentLine.split(",")[0].split(";")[0].replace("[", ""))
        yPosition = float(currentLine.split(",")[1].split(";")[1].replace("]", ""))

        if xPosition < xMiddle:
            if yPosition > yMiddle:
                topLeftLines.append(currentLine)
            if yPosition <= yMiddle:
                bottomLeftLines.append(currentLine)
        if xPosition >= xMiddle:
            if yPosition > yMiddle:
                topRightLines.append(currentLine)
            if yPosition <= yMiddle:
                bottomRightLines.append(currentLine)

    outputFileOne = open(topLeftFilename, "w")
    outputString = "\n".join(topLeftLines)
    outputFileOne.write(outputString)
    outputFileOne.close()

    outputFileTwo = open(topRightFilename, "w")
    outputString = "\n".join(topRightLines)
    outputFileTwo.write(outputString)
    outputFileTwo.close()

    outputFileThree = open(bottomLeftFilename, "w")
    outputString = "\n".join(bottomLeftLines)
    outputFileThree.write(outputString)
    outputFileThree.close()

    outputFileFour = open(bottomRightFilename, "w")
    outputString = "\n".join(bottomRightLines)
    outputFileFour.write(outputString)
    outputFileFour.close()

    print(i, "/", fileCount)
