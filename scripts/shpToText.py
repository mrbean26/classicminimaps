import shapefile
import glob

allFiles = glob.glob("*Link.shp")
for f in range(len(allFiles)):
    newShapeFile = shapefile.Reader(allFiles[f])
    output = ""

    shapes = newShapeFile.shapes()
    shapeCount = len(shapes)

    for i in range(shapeCount):
        points = shapes[i].points
        pointCount = len(points)

        for p in range(pointCount):
            output += str(points[p][0]) + ","
            output += str(points[p][1]) + ","
        output = output[:-1]    
        output += "\n"

    newfile = open("output/" + allFiles[f] + ".txt", mode="w+")
    newfile.write(output)
    newfile.close()

    print(f + 1, "/", len(allFiles))

    
    
