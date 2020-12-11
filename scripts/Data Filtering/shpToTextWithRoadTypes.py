import shapefile
import dbfread
import glob

allFiles = glob.glob("*Link.shp")
for f in range(len(allFiles)):
    newShapeFile = shapefile.Reader(allFiles[f])
    dbfName = allFiles[f].replace(".shp", ".dbf")
    newDataBase = dbfread.DBF(dbfName, load=True).records

    output = ""
    shapes = newShapeFile.shapes()
    shapeCount = len(shapes)

    newfile = open("output/" + allFiles[f] + ".txt", mode="w+")
    newfile.close()

    newfile = open("output/" + allFiles[f] + ".txt", mode="a")
    
    for i in range(shapeCount):
        points = shapes[i].points
        pointCount = len(points)
        output += newDataBase[i]['function'] + ","
        for p in range(pointCount):
            output += str(points[p][0]) + ","
            output += str(points[p][1]) + ","
        output = output[:-1]    
        output += "\n"

        if i % 50 == 0 and i > 0:
            newfile.write(output)
            output = "" # saves memory and speeds up significantly

    newfile.write(output)
    newfile.close()
    print(f + 1, "/", len(allFiles))

    
    
