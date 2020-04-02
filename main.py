import gridMath
import shapefile
allRivers = shapefile.Reader("data/WatercourseLink.shp")

allShapes = allRivers.shapes()
shapeCount = len(allShapes)

from tkinter import *
window = Tk()
c = Canvas(window, width = 360, height = 640, bg='black')
c.pack()

import location
gridThisLocation = gridMath.latLongToGrid(location.getLatLong())


myRoads = shapefile.Reader("data/TQ_RoadLink.shp")
allShapesRoads = myRoads.shapes()
shapesInRoads = len(allShapesRoads)

for i in range(shapesInRoads):
	points = allShapesRoads[i].points
	pointcount = len(points)

	for p in range(pointcount - 1):
		x1 = points[p][0] / 100 - gridThisLocation[0] / 100
		y1 = points[p][1] / 100 - gridThisLocation[1] / 100

		x2 = points[p + 1][0] / 100 - gridThisLocation[0] / 100
		y2 = points[p + 1][1] / 100 - gridThisLocation[1] / 100

		c.create_line(x1, y1, x2, y2, fill='blue')




c.mainloop()
