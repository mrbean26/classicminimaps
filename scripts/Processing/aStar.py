# use connection file as input
#https://gist.github.com/Nicholas-Swift/003e1932ef2804bebef2710527008f44
import time

class Node(): # float position, include file indexes?
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

# pass dictionary to aStar() with position as string with connections
# (from createOutput.py)
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

# create dictionary
fileOpened = open("outputConnections/TQ_RoadLink.shp.txt")
lines = fileOpened.readlines()
fileOpened.close()

outputDictionary = {}
for line in lines:
    data = line.strip().split(",")
    key = data[0]

    data.pop(0)
    outputDictionary[key] = data


print("dictionary loaded")
startNode = "[500670.0;200154.0]"
endNode = "[601016.49;118791.33]"
startTime = time.time()
path = aStar(startNode, endNode, outputDictionary)

print(time.time() - startTime)
print("Found Path")
print(path)
