# technical spec here : https://www.ordnancesurvey.co.uk/documents/os-open-names-getting-started-guide.pdf
import glob
import os

allFiles = glob.glob("*.csv")
fileCount = len(allFiles)
done = False
for i in range(fileCount):
    print("Starting:", i + 1, " / ", fileCount)
    currentFileName = allFiles[i]

    # create output file for this square if not already
    outputName = "output/" + currentFileName[0 : 2] + "Names.txt"
    if os.path.exists(outputName) == False:
        newFile = open(outputName, 'w+')
        newFile.close()

    # open file for appening
    outputFile = open(outputName, 'a', encoding="utf-8")
    readingFile = open(currentFileName, 'r', encoding="utf-8")

    allLines = readingFile.readlines()
    for line in allLines:
        currentLine = line.strip()
        data = currentLine.split(",")

        if "postcodeunit" not in data[1]: # words, not postcode
            name = data[2]
            xPos = data[8]
            yPos = data[9]

            currentIndex = 8
            while xPos.replace(".", "").isdecimal() == False:
                currentIndex += 1
                xPos = data[currentIndex]
                yPos = data[currentIndex + 1]

            output = name
            output = output + ","
            output = output + xPos
            output = output + ","
            output = output + yPos
            output = output + "\n"
            outputFile.write(output)
        
    
    
