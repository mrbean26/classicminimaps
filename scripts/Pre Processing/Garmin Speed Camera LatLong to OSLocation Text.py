# math
from math import tan, sin, cos, radians

def tanSquared(x):
    return tan(x) * tan(x)

def reciprocalCos(x):
    return 1 / cos(x)

def sinSquared(x):
    return sin(x) * sin(x)

def latLongToGrid(latLng):
     OSGB_F0 = 0.9996012717
     N0 = -100000.0
     E0 = 400000.0
     phi0 = radians(49.0)
     lambda0 = radians(-2.0)
     a = 6377563.396
     b = 6356256.909
     eSquared = ((a*a) - (b*b)) / (a*a)
     phi = radians(latLng[0])
     lambda1 = radians(latLng[1])
     E = 0.0
     N = 0.0
     n = (a - b) / (a + b)
     v = a * OSGB_F0 * pow(1.0 - eSquared * sinSquared(phi), -0.5)
     rho = a * OSGB_F0 * (1.0 - eSquared) * pow(1.0 - eSquared * sinSquared(phi), -1.5)
     etaSquared = (v / rho) - 1.0
     M = (b * OSGB_F0) * (((1 + n + ((5.0 / 4.0) * n * n) + ((5.0 / 4.0) * n * n * n)) * (phi - phi0)) - (((3 * n) + (3 * n * n) + ((21.0 / 8.0) * n * n * n)) * sin(phi - phi0) * cos(phi + phi0)) + ((((15.0 / 8.0) * n * n) + ((15.0 / 8.0) * n * n * n)) * sin(2.0 * (phi - phi0)) * cos(2.0 * (phi + phi0))) - (((35.0 / 24.0) * n * n * n) * sin(3.0 * (phi - phi0)) * cos(3.0 * (phi + phi0))))
     I = M + N0
     II = (v / 2.0) * sin(phi) * cos(phi)
     III = (v / 24.0) * sin(phi) * pow(cos(phi), 3.0) * (5.0 - tanSquared(phi) + (9.0 * etaSquared))
     IIIA = (v / 720.0) * sin(phi) * pow(cos(phi), 5.0) * (61.0 - (58.0 * tanSquared(phi)) + pow(tan(phi), 4.0))
     IV = v * cos(phi)
     V = (v / 6.0) * pow(cos(phi), 3.0) * ((v / rho) - tanSquared(phi))
     VI = (v / 120.0)* pow(cos(phi), 5.0) * (5.0 - (18.0 * tanSquared(phi)) + (pow(tan(phi), 4.0)) + (14 * etaSquared) - (58 * tanSquared(phi) * etaSquared))
     N = I + (II * pow(lambda1 - lambda0, 2.0)) + (III * pow(lambda1 - lambda0, 4.0)) + (IIIA * pow(lambda1 - lambda0, 6.0))
     E = E0 + (IV * (lambda1 - lambda0)) + (V * pow(lambda1 - lambda0, 3.0)) + (VI * pow(lambda1 - lambda0, 5.0))

     return [E, N]
# main
import glob
allFiles = glob.glob("*.csv")
fileCount = len(allFiles)

outputCreationFile = open("output.txt", "w+")
outputCreationFile.close()

for f in range(fileCount):
    print("Starting: ", f + 1, "/", fileCount)
    currentFile = open(allFiles[f], "r")
    allLines = currentFile.readlines()
    currentFile.close()

    for l in allLines:
        currentLine = l.strip()
        data = currentLine.split(", ")

        lat = float(data[1])
        long = float(data[0])

        gridReference = latLongToGrid([lat, long])
        newLine = str(gridReference[0]) + "," + str(gridReference[1]) + "\n"

        outputFile = open("output.txt", 'a')
        outputFile.write(newLine)
        outputFile.close()
