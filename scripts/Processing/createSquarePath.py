# Functions
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
def nearestPointAverages(originalPoint):
    lowestDistance = 10000000
    currentNearestPoint = ""

    fileOpened = open("outputAverages.txt", "r")
    lines = fileOpened.readlines()
    fileOpened.close()

    for line in lines:
        currentLine = line.strip()
        currentPoint = currentLine.split(",")[1]

        distanceCurrent = distance(currentPoint, originalPoint)
        if distanceCurrent < lowestDistance:
            lowestDistance = distanceCurrent
            currentNearestPoint = currentPoint

    return currentNearestPoint

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
            if child in closedList: # may cause errors
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

# main
mainPointPath = []
startPoint = "[261301.36104478076;847352.2813044911]"
endPoint = "[539478.9954586509;163616.24599880082]"

# load square dictionaries, by finding nearest 8 squares
fileOpened = open("mapSquarePointConnections.txt")
lines = fileOpened.readlines()
fileOpened.close()

squareDictionary = {}
for line in lines:
    data = line.strip().split(",")
    key = data[0]

    data.pop(0)
    squareDictionary[key] = data

# find what square start & end nodes sits in (find midpoint)
nearestPointStart = nearestPointAverages(startPoint)
nearestPointEnd = nearestPointAverages(endPoint)

squarePath = aStar(nearestPointStart, nearestPointEnd, squareDictionary)
if len(squarePath) > 1:
	squarePath[0] = startPoint
	squarePath[len(squarePath) - 1] = endPoint

print(squarePath)
