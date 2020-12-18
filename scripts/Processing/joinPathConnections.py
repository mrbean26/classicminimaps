# general functions
from math import sqrt
def distanceString(pointOne, pointTwo):
    one = pointOne.replace("[", "").replace("]", "")
    two = pointTwo.replace("[", "").replace("]", "")

    oneData = one.split(";")
    twoData = two.split(";")

    x1 = float(oneData[0])
    y1 = float(oneData[1])

    x2 = float(twoData[0])
    y2 = float(twoData[1])

    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2))

def generateDictionaryFromConnectionFile(fileName):
    fileOpened = open(fileName)
    connectionLines = fileOpened.readlines()
    fileOpened.close()

    outputDictionary = {}
    for line in connectionLines:
        data = line.strip().split(",")
        key = data[0]

        data.pop(0)
        outputDictionary[key] = data
    return outputDictionary

def findNearestPoint(point, connectionFileFilename):
    openedFile = open(connectionFileFilename)
    connectionLines = openedFile.readlines()
    openedFile.close()

    lowestDistance = 10000000
    currentNearestPoint = ""

    for line in connectionLines:
        currentLine = line.strip()
        currentPoint = currentLine.split(",")[0]

        distanceCurrent = distanceString(currentPoint, point)
        if distanceCurrent < lowestDistance:
            lowestDistance = distanceCurrent
            currentNearestPoint = currentPoint

            return currentNearestPoint

def generateConnectionFilename(letters):
    return "outputConnections/" + letters + "_RoadLink.shp.txt"

# A*
class Node():
    def __init__(self, parent = None, position = None):
        self.parent = parent
        self.position = position

        data = position.replace("[", "").replace("]", "").split(";")
        self.xPosition = float(data[0])
        self.yPosition = float(data[1])

        self.g = 0
        self.h = 0
        self.f = 0

    def __eq__(self, other):
        return self.position == other.position

def aStar(stringStart, stringEnd, connectionDict):
    startNode = Node(None, stringStart)
    endNode = Node(None, stringEnd)

    openList = [startNode]
    closedList = []

    while len(openList) > 0:
        currentNode = openList[0]
        currentIndex = 0

        for index, item in enumerate(openList):
            if item.f < currentNode.f:
                currentNode = item
                currentIndex = index

        openList.pop(currentIndex)
        closedList.append(currentNode)

        if currentNode == endNode:
            path = []
            current = currentNode

            while current is not None:
                path.append(current.position)
                current = current.parent
            return path[::-1]

        # generate children
        try:
            childrenPositions = connectionDict[currentNode.position]
        except:
            continue
        children = []

        for childPosition in childrenPositions:
            children.append(Node(currentNode, childPosition))

        # loop children
        for child in children:
            if child in closedList:
                continue

            child.g = currentNode.g + 1
            child.h = ((child.xPosition - endNode.xPosition) ** 2) + ((child.yPosition - endNode.yPosition) ** 2)
            child.f = child.g + child.h

            skip = False
            for openNode in openList:
                if child == openNode and child.g > openNode.g:
                    skip = True
                    break

            if skip == True:
                continue

            openList.append(child)

# specific functions
def findParentSquare(point):
    openedFile = open("squareMinMax.txt")
    boundaryLines = openedFile.readlines()
    openedFile.close()

    pointData = point.replace("[", "").replace("]", "").split(";")
    pointX = float(pointData[0])
    pointY = float(pointData[1])

    for line in boundaryLines:
        currentLine = line.strip()
        currentData = currentLine.split(",")

        currentXMin = float(currentData[1])
        currentXMax = float(currentData[2])

        currentYMin = float(currentData[3])
        currentYMax = float(currentData[4])

        if pointX > currentXMin and pointX < currentXMax:
            if pointY > currentYMin and pointY < currentYMax:
                return currentData[0]

def getSquareAverageFromLetters(letters):
    openedFile = open("outputAverages.txt")
    averagesLines = openedFile.readlines()
    openedFile.close()

    for line in averagesLines:
        currentLine = line.strip()
        currentData = currentLine.split(",")

        if currentData[0] == letters:
            return currentData[1]

def createLargerSquarePath(startNode, endNode):
    startSquare = findParentSquare(startNode)
    endSquare = findParentSquare(endNode)

    startAverage = getSquareAverageFromLetters(startSquare)
    endAverage = getSquareAverageFromLetters(endSquare)

    if startSquare == endSquare:
        return [[startSquare, startNode, endNode]]

    mapSquareConnectionDictionary = generateDictionaryFromConnectionFile("mapSquarePointConnections.txt")
    squarePath = aStar(startAverage, endAverage, mapSquareConnectionDictionary)

    squarePath[0] = startNode
    squarePath[len(squarePath) - 1] = endNode

    returnedPath = []
    squarePathCount = len(squarePath)

    for i in range(squarePathCount):
        currentPoint = squarePath[i]

        if i == 0:
            newMiniPath = [startSquare]
            newMiniPath.append(startNode)
            newMiniPath.append(findNearestPoint(squarePath[i + 1], generateConnectionFilename(startSquare)))

            returnedPath.append(newMiniPath)
        if i > 0 and i < squarePathCount - 1:
            newMiniPath = [findParentSquare(squarePath[i])]
            newMiniPath.append(findNearestPoint(returnedPath[len(returnedPath) - 1][2], generateConnectionFilename(newMiniPath[0])))
            newMiniPath.append(findNearestPoint(squarePath[i + 1], generateConnectionFilename(newMiniPath[0])))

            returnedPath.append(newMiniPath)
        if i == squarePathCount - 1:
            newMiniPath = [endSquare]
            newMiniPath.append(findNearestPoint(returnedPath[len(returnedPath) - 1][2], generateConnectionFilename(newMiniPath[0])))
            newMiniPath.append(endNode)

            returnedPath.append(newMiniPath)

    return returnedPath

def returnFullPath(startNode, endNode):
    miniPaths = createLargerSquarePath(startNode, endNode)

    returnedPath = []
    index = 0
    for miniPath in miniPaths:
        index += 1
        print(index, len(miniPaths))

        currentDictionary = generateDictionaryFromConnectionFile(generateConnectionFilename(miniPath[0]))
        returnedPath.append(aStar(miniPath[1], miniPath[2], currentDictionary))

    return returnedPath

print("Starting now...")
import time
startTime = time.time()

startNode = "[399392.0;306447.0]"
endNode = "[200109.0;196940.0]"

print(returnFullPath(startNode, endNode))
print("Finished in", time.time() - startTime, "seconds")
