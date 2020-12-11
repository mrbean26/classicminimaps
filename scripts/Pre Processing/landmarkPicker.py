from tkinter import *
from glob import glob
windowSize = 600

def drawPoint(x, y, canvas):
    canvas.create_oval(x, y, x + 1, y + 1)
def findMinMaxX(lines):
    minX = float(lines[0].split(",")[0].split(";")[0].replace("[", ""))
    maxX = float(lines[len(lines) - 1].split(",")[0].split(";")[0].replace("[", ""))

    return minX, maxX
def findMinMaxY(lines):
    minY = 1000000000
    maxY = 0

    for line in lines:
        yPosition = float(line.split(",")[0].split(";")[1].replace("]", ""))

        if yPosition > maxY:
            maxY = yPosition
        if yPosition < minY:
            minY = yPosition

    return minY, maxY

currentFilename = ""
minX = 0
maxX = 0
minY = 0
maxY = 0

def getOrigin(eventorigin):
    global currentFilename
    x = eventorigin.x
    y = eventorigin.y

    x = (x / windowSize) * (maxX - minX) + minX
    y = (y / windowSize) * (maxY - minY) + minY

    newFile = open(currentFilename.replace("roadPointAllRoadConnections", "outputLandmarks"), "a")
    newFile.write("[" + str(x) + ";" + str(y) + "]\n")
    newFile.close()

fileNames = glob("roadPointAllRoadConnections/*.shp.txt")
fileCount = len(fileNames)

for f in range(fileCount):
    fileName = fileNames[f]
    currentFilename = fileName

    window = Tk()
    c = Canvas(window, width = windowSize, height = windowSize)
    c.pack()

    openedFile = open(fileName)
    openedLines = openedFile.readlines()
    openedFile.close()

    minX, maxX = findMinMaxX(openedLines)
    minY, maxY = findMinMaxY(openedLines)

    multiplierX = (maxX - minX) * windowSize
    multiplierY = (maxY - minY) * windowSize

    for line in openedLines:
        point = line.split(",")[0]
        x = float(point.split(";")[0].replace("[", ""))
        y = float(point.split(";")[1].replace("]", ""))

        x = ((x - minX) / (maxX - minX)) * windowSize
        y = ((y - minY) / (maxY - minY)) * windowSize

        drawPoint(x, windowSize - y, c)

    window.bind("<Button 1>", getOrigin)
    c.mainloop()
