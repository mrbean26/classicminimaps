import gridMath
import shapefile
allRivers = shapefile.Reader("data/WatercourseLink.shp")

allShapes = allRivers.shapes()
shapeCount = len(allShapes)

from tkinter import *
window = Tk()
c = Canvas(window, width = 360, height = 640, bg='black')
c.pack()

print(allShapes[0].points)

for i in range(shapeCount):
    currentPoints = allShapes[i].points
    pointCount = len(currentPoints)

    for p in range(pointCount - 1):
        c.create_line(currentPoints[p][0], currentPoints[p][1], currentPoints[p + 1][0], currentPoints[p + 1][1], fill='lightblue')

c.mainloop()
