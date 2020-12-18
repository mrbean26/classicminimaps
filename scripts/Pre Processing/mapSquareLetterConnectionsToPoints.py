# create square letter dictionary
letterDictionary = {}
openedFile = open("outputAverages.txt")
lines = openedFile.readlines()
openedFile.close()

for line in lines:
    currentLine = line.strip()
    data = currentLine.split(",")

    letterDictionary[data[0]] = data[1]

# create point connections
outputDictionary = {}
openedFile = open("mapSquareConnections.txt")
lines = openedFile.readlines()
openedFile.close()
print(letterDictionary.keys())
for line in lines:
    currentLine = line.strip()
    data = currentLine.split(",")

    valueList = []
    for i in range(1, len(data)):
        print(data[i])
        valueList.append(letterDictionary[data[i]])

    outputDictionary[letterDictionary[data[0]]] = valueList

# write to file
outputString = ""
allKeys = list(outputDictionary.keys())

for key in allKeys:
    outputString += key + ","

    for item in outputDictionary[key]:
        outputString += item + ","

    outputString = outputString[:-1] + "\n"

outputFile = open("mapSquarePointConnections.txt", "w+")
outputFile.write(outputString)
outputFile.close()
