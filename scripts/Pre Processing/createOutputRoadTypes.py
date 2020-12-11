import sys

fileName = sys.argv[1]
file = open(fileName, "r")
lines = file.readlines()
file.close()

# create files
allRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_allRoads.txt", "w+")
motorwayRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_motorwayRoads.txt", "w+")
aRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_aRoads.txt", "w+")
bRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_bRoads.txt", "w+")
minorRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_minorRoads.txt", "w+")
localRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_localRoads.txt", "w+")
secondaryRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_secondaryRoads.txt", "w+")

# open files for appending
allRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_allRoads.txt", "a")
motorwayRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_motorwayRoads.txt", "a")
aRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_aRoads.txt", "a")
bRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_bRoads.txt", "a")
minorRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_minorRoads.txt", "a")
localRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_localRoads.txt", "a")
secondaryRoads = open("roadOutput/" + fileName.replace(".shp.txt", "") + "_secondaryRoads.txt", "a")

# dictionaries
allPoints = {}
motorwayPoints = {}
aPoints = {}
bPoints = {}
minorPoints = {}
localPoints = {}
secondaryPoints = {}

# read lines
for line in lines:
    currentLine = line.strip()
    data = currentLine.split(",")
    dataCount = len(data)

    # get dictionary
    currentDictionary = None
    if data[0] == "Motorway":
        currentDictionary = motorwayPoints
    if data[0] == "A Road":
        currentDictionary = aPoints
    if data[0] == "B Road":
        currentDictionary = bPoints
    if data[0] == "Minor Road":
        currentDictionary = minorPoints
    if data[0] == "Local Road" or data[0] == "Local Access Road" or data[0] == "Restricted Local Access Road":
        currentDictionary = localPoints
    if data[0] == "Secondary Access Road":
        currentDictionary = secondaryPoints

    # points
    
    for i in range(1, dataCount):
        if i % 2 == 0: # y point
            continue

        currentKey = "[" + data[i] + ";" + data[i + 1] + "]"
        if currentKey not in currentDictionary:
            currentDictionary.update({currentKey : []})

        if currentKey not in allPoints:
            allPoints.update({currentKey : []})

        newValues = []
        if i > 2 and i < dataCount - 2: # in the middle
            newValues.append("[" + data[i - 2] + ";" + data[i - 1] + "]")
            newValues.append("[" + data[i + 2] + ";" + data[i + 3] + "]")
        if i == 0:
            newValues.append("[" + data[i + 2] + ";" + data[i + 3] + "]")
        if i == dataCount - 2:
            newValues.append("[" + data[i - 2] + ";" + data[i - 1] + "]")

        for value in newValues:
            currentDictionary[currentKey].append(value)
            allPoints[currentKey].append(value)

    # set new dictionary
    if data[0] == "Motorway":
        motorwayPoints = currentDictionary
    if data[0] == "A Road":
        aPoints = currentDictionary
    if data[0] == "B Road":
        bPoints = currentDictionary
    if data[0] == "Minor Road":
        minorPoints = currentDictionary
    if data[0] == "Local Road" or data[0] == "Local Access Road" or data[0] == "Restricted Local Access Road":
        localPoints = currentDictionary
    if data[0] == "Secondary Access Road":
        secondaryPoints = currentDictionary

# outputs
outputAll = ""
outputMotorway = ""
outputA = ""
outputB = ""
outputMinor = ""
outputLocal = ""
outputSecondary = ""

index = 0
for key in allPoints.keys():
    index+=1
    outputAll += key + ","

    for item in allPoints[key]:
        outputAll += item + ","

    outputAll = outputAll[:-1] + "\n"

    if index % 40 == 0:
        allRoads.write(outputAll)
        outputAll = ""

index = 0
for key in motorwayPoints.keys():
    index+=1
    outputMotorway += key + ","

    for item in motorwayPoints[key]:
        outputMotorway += item + ","

    outputMotorway = outputMotorway[:-1] + "\n"

    if index % 40 == 0:
        motorwayRoads.write(outputMotorway)
        outputMotorway = ""

index = 0
for key in aPoints.keys():
    index+=1
    outputA += key + ","

    for item in aPoints[key]:
        outputA += item + ","

    outputA = outputA[:-1] + "\n"

    if index % 40 == 0:
        aRoads.write(outputA)
        outputA = ""

index = 0
for key in bPoints.keys():
    index+=1
    outputB += key + ","

    for item in bPoints[key]:
        outputB += item + ","

    outputB = outputB[:-1] + "\n"

    if index % 40 == 0:
        bRoads.write(outputB)
        outputB = ""

index = 0
for key in minorPoints.keys():
    index+=1
    outputMinor += key + ","

    for item in minorPoints[key]:
        outputMinor += item + ","

    outputMinor = outputMinor[:-1] + "\n"

    if index % 40 == 0:
        minorRoads.write(outputMinor)
        outputMinor = ""

index = 0
for key in localPoints.keys():
    outputLocal += key + ","
    index+=1

    for item in localPoints[key]:
        outputLocal += item + ","

    outputLocal = outputLocal[:-1] + "\n"

    if index % 40 == 0:
        localRoads.write(outputLocal)
        outputLocal = ""

index = 0
for key in secondaryPoints.keys():
    index+=1
    outputSecondary += key + ","

    for item in secondaryPoints[key]:
        outputSecondary += item + ","

    outputSecondary = outputSecondary[:-1] + "\n"

    if index % 40 == 0:
        secondaryRoads.write(outputSecondary)
        outputSecondary = ""



