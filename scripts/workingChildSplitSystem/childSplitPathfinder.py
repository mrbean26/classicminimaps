import time

file = open("output.txt", "r")
lines = file.readlines()
file.close()

def getIndex(name):
        lineCount = len(lines)
        for i in range(lineCount):
                currentLine = lines[i].strip()
                data = currentLine.split(",")

                if data[0] == name:
                        return i

positionsAlreadySeen = []
class node:
    def __init__(self, parentIndexPath, indexFromParent, position):
        self.position = position
        self.indexPath = parentIndexPath + [indexFromParent]

    def getConnections(self):
        lineIndex = getIndex(self.position)
        line = lines[lineIndex].strip()

        data = line.split(",")
        dataCount = len(data)

        returned = []
        for i in range(1, dataCount):
            position = data[i]
            alreadySeen = False

            for item in positionsAlreadySeen:
                if item == position:
                    alreadySeen = True
                    break

            if alreadySeen == False:
                returned.append(node(self.indexPath, i, position))

        return returned

def findPath(startPosition, endPosition):
    startNode = node([], 0, startPosition)
    endNode = node([], 0, endPosition)

    startNode.indexPath = []
    startTime = time.time()
    time.sleep(1) # only for debugging to not round numbers
    connections = [startNode]
    total = 0
    while True:
        total += len(connections)
        print(total)
        for connection in connections:
            positionsAlreadySeen.append(connection.position)
            if connection.position == endNode.position:
                print("Found path in " + str(time.time() - startTime - 1) + " seconds!") # -1 just for debugging to not round numbers
                return connection.indexPath

        newNodes = []
        count = len(connections)
        for i in range(count):
            newConnections = connections[i].getConnections()
            newNodes = newNodes + newConnections

        if len(newNodes) == 0:
                print("No path!")
                break

        connections = newNodes

def getCoordsFromPath(startPosition, usedPath):
    coords = [startPosition]
    currentNode = startPosition

    for index in usedPath:
        currentLine = lines[getIndex(currentNode)].strip()
        data = currentLine.split(",")

        nextNode = data[index]
        coords.append(nextNode)
        currentNode = nextNode

    return coords

start = "[572715.98;199161.34]"
end = "[533757.06;187730.34]"

path = findPath(start, end)
if path is not None:
	print(getCoordsFromPath(start, path))
