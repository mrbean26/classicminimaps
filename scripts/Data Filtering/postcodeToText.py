import glob
allFiles = glob.glob("*.csv")
fileCount = len(allFiles)

for i in range(fileCount):
    print("Starting: " + str(i + 1) + " / " + str(fileCount))
    
    # create output file
    outputFileNew = open("output/" + allFiles[i] + ".txt", mode="w+")
    outputFileNew.close()
    # create output
    output = ""
    
    readingFile = open(allFiles[i], mode="r")
    allLines = readingFile.readlines()
    
    lineCount = len(allLines)
    for l in range(lineCount):
        currentLine = allLines[l].strip()
        data = currentLine.split(",")

        postcode = data[0].replace("\"", "", 2).replace(" ", "")
        easting = data[2]
        northing = data[3]

        output = output + postcode + "," + easting + "," + northing + "," + "\n"
    readingFile.close()
    # write output
    outputFile = open("output/" + allFiles[i] + ".txt", mode="w")
    outputFile.write(output)
    outputFile.close()
