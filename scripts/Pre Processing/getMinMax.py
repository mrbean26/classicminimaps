import re

file = open("textPointsAveragesMinsMaxes.txt", "r")
allLines = file.readlines()
file.close()

outputFile = open("textPointsMinMax.txt", "a")

for line in allLines:
    currentLine = line.strip()

    newLine = currentLine[0] + currentLine[1] + ","
    newLine = newLine + re.search("xmin: (.*) xmax", currentLine).group(1) + "," #xmin
    newLine = newLine + re.search("ymin: (.*) ymax", currentLine).group(1) + ","
    newLine = newLine + re.search("xmax: (.*) ymin", currentLine).group(1) + "," #xmax
    newLine = newLine + re.search("ymax: (.*) xavg", currentLine).group(1) + "\n"

    outputFile.write(newLine)
outputFile.close()
