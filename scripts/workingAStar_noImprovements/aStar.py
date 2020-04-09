from time import time

def positionToFloats(position):
    first = position.replace("[", "").replace("]", "")
    second = first.split(";")

    return [float(second[0]), float(second[1])]

def getIndex(positionName, lines):
    lineCount = len(lines)
    for i in range(lineCount):
        currentLine = lines[i].strip()
        data = currentLine.split(",")

        if data[0] == positionName:
            return i

def distance(positionOneString, positionTwoString):
    positionOneFloat = positionToFloats(positionOneString)
    positionTwoFloat = positionToFloats(positionTwoString)

    xDif = positionOneFloat[0] - positionTwoFloat[0]
    yDif = positionOneFloat[1] - positionTwoFloat[1]

    result = pow(xDif, 2.0) + pow(yDif, 2.0)
    return result

class aStarNode:
    def __init__(self, parent, position):
        self.parent = parent
        self.position = position

        self.gCost = 0
        self.hCost = 0
        self.fCost = 0

    def __eq__(self, other):
        if other == None:
            return False
        return self.position == other.position

    def getChildren(self, lines):
        lineIndex = getIndex(self.position, lines)
        line = lines[lineIndex].strip()

        data = line.split(",")
        return data

def findPath(start, end, fileName):
    file = open(fileName, mode="r")
    lines = file.readlines()
    file.close()
        
    startNode = aStarNode(None, start)
    endNode = aStarNode(None, end)

    openList = []
    closedList = []

    seenAlready = []

    openList.append(startNode)
    while len(openList) > 0:
        currentNode = openList[0]
        currentIndex = 0

        openListCount = len(openList)
        for i in range(openListCount):
            seenAlready.append(openList[i].position)
            item = openList[i]
            if item.fCost < currentNode.fCost:
                currentNode = item
                currentIndex = i

        openList.pop(currentIndex)
        closedList.append(currentNode)

        if currentNode == endNode:
            path = []
            current = currentNode

            while current != None:
                path.append(current.position)
                current = current.parent

            path.reverse()
            return path

        connections = currentNode.getChildren(lines)
        children = []
        for newPositionString in connections:
            if newPositionString in seenAlready:
                continue

            newNode = aStarNode(currentNode, newPositionString)
            children.append(newNode)

        for child in children:
            if child in closedList:
                continue

            child.gCost = currentNode.gCost + 1
            child.hCost = distance(child.position, endNode.position)
            child.fCost = child.gCost + child.hCost

            openCountNew = len(openList)
            continueBool = False
            for on in range(openCountNew):
                if child == openList[on] and child.gCost > openList[on].gCost:
                    continueBool = True

            if continueBool == True:
                continue
            openList.append(child)
    return None

startPos = input("Enter start position: ")
endPos = input("Enter end position: ")
startTime = time()

path = findPath(startPos, endPos, "output.txt")
print("Path coords: ", path)

print("Finished in " + str(time() - startTime) + " seconds!")
            
            
        
