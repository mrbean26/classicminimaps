import platform
import os
import subprocess

def getOS():
	return platform.system()

def getLatLong():
	osType = getOS()

	if osType == "Darwin": # mac
		relativePath = os.path.dirname(os.path.abspath("executables/whereami")) + "/"
		output = subprocess.check_output([relativePath + "./whereami"]).decode()
		allOutputLines = output.splitlines()

		if "Latitude" not in output:
			return None

		latitute = float(allOutputLines[0].split(": ")[1])
		longitude = float(allOutputLines[1].split(": ")[1])

		return [latitute, longitude]
