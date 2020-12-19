#include "classicminiGraphics.h"
#include "classicminimaps.h"
#include "functions.h"
#include "mapInterface.h"
#include <thread>

#include "classicminiPathfinding.h"

int main(void){
	if (!classicminigraphics::begin(1280, 720, "Maps", false)) {
		return -1;
	}
	texture::begin();
	classicminimaps::begin();
	mapInterface::begin();
	text::begin(200);

	specific::findAndLoadPath("[519927.0;148071.0]", "[502140.99;150639.24]");

	while (!glfwWindowShouldClose(classicminigraphics::window)) {
		classicminigraphics::startEvents();
		
		classicminimaps::mainloop();
		mapInterface::mainloop();
		specific::drawPath();
		classicminigraphics::endEvents();
	}
	glfwTerminate();
}