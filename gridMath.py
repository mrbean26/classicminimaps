# Grid reference to LatLng = https://github.com/xni06/JCoord/blob/master/src/main/java/uk/me/jstott/jcoord/OSRef.java
from math import tan, cos, sin, degrees, radians

def tanSquared(x):
    return tan(x) * tan(x)

def reciprocalCos(x):
    return 1 / cos(x)

def sinSquared(x):
    return sin(x) * sin(x)

def gridToLatLong(grid):
    OSGB_F0 = 0.9996012717
    N0 = -100000.0
    E0 = 400000.0
    phi0 = radians(49.0)
    lambda0 = radians(-2.0)

    a = 6377563.396
    b = 6356256.909
    eSquared = (a - b) / a
    phi = 0.0
    lambda1 = 0.0
    E = grid[0]
    N = grid[1]
    n = (a - b) / (a + b)
    M = 0.0
    phiPrime = ((N - N0) / (a * OSGB_F0)) + phi0

    while ((N - N0 - M) >= 0.001):
            M = (b * OSGB_F0) * (((1 + n + ((5.0 / 4.0) * n * n) + ((5.0 / 4.0) * n * n * n)) * (phiPrime - phi0)) - (((3 * n) + (3 * n * n) + ((21.0 / 8.0) * n * n * n)) * sin(phiPrime - phi0) * cos(phiPrime + phi0)) + ((((15.0 / 8.0) * n * n) + ((15.0 / 8.0) * n * n * n)) * sin(2.0 * (phiPrime - phi0)) * cos(2.0 * (phiPrime + phi0))) - (((35.0 / 24.0) * n * n * n) * sin(3.0 * (phiPrime - phi0)) * cos(3.0 * (phiPrime + phi0))))
            phiPrime += (N - N0 - M) / (a * OSGB_F0)

    v = a * OSGB_F0 * pow(1.0 - eSquared * sinSquared(phiPrime), -0.5)
    rho = a * OSGB_F0 * (1.0 - eSquared) * pow(1.0 - eSquared * sinSquared(phiPrime), -1.5)
    etaSquared = (v / rho) - 1.0
    VII = tan(phiPrime) / (2 * rho * v)
    VIII = (tan(phiPrime) / (24.0 * rho * pow(v, 3.0))) * (5.0 + (3.0 * tanSquared(phiPrime)) + etaSquared - (9.0 * tanSquared(phiPrime) * etaSquared))
    IX = (tan(phiPrime) / (720.0 * rho * pow(v, 5.0))) * (61.0 + (90.0 * tanSquared(phiPrime)) + (45.0 * tanSquared(phiPrime) * tanSquared(phiPrime)))
    X = reciprocalCos(phiPrime) / v
    XI = (reciprocalCos(phiPrime) / (6.0 * v * v * v)) * ((v / rho) + (2 * tanSquared(phiPrime)))
    XII = (reciprocalCos(phiPrime) / (120.0 * pow(v, 5.0))) * (5.0 + (28.0 * tanSquared(phiPrime)) + (24.0 * tanSquared(phiPrime) * tanSquared(phiPrime)))
    XIIA = (reciprocalCos(phiPrime) / (5040.0 * pow(v, 7.0))) * (61.0 + (662.0 * tanSquared(phiPrime)) + (1320.0 * tanSquared(phiPrime) * tanSquared(phiPrime)) + (720.0 * tanSquared(phiPrime) * tanSquared(phiPrime) * tanSquared(phiPrime)))

    phi = phiPrime - (VII * pow(E - E0, 2.0)) + (VIII * pow(E - E0, 4.0)) - (IX * pow(E - E0, 6.0))
    lambda1 = lambda0 + (X * (E - E0)) - (XI * pow(E - E0, 3.0)) + (XII * pow(E - E0, 5.0)) - (XIIA * pow(E - E0, 7.0))

    return [degrees(phi), degrees(lambda1)]

def latLongToGrid(latLng):
     OSGB_F0 = 0.9996012717
     N0 = -100000.0
     E0 = 400000.0
     phi0 = radians(49.0)
     lambda0 = radians(-2.0)
     a = 6377563.396
     b = 6356256.909
     eSquared = (a - b) / a
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
