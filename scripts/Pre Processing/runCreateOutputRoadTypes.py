import glob
import subprocess


for file in glob.glob("*.shp.txt"):
    subprocess.call("python createOutputRoadTypes.py " + file)
