#pragma once

#ifndef MAP_INTERFACE_H
#define MAP_INTERFACE_H

#include "functions.h"
#include "classicminiGraphics.h"
#include "classicminimaps.h"

namespace mapInterface {
	extern bool interfaceOpen;
	extern string postcodeText;
	extern vec2 postcodesLocation;

	void mainloop();
	void begin();

	void openInterface();
	void closeInterface();
	void updatePostcodeText(int key, int mode);
	void renderNonInterface(float indentMultiplier, float textSize, float gapMultiplier);
	void renderInterfaceItems(float textSize, float gapMultiplier);

}

#endif

