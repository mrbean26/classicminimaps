#include "classicminiGraphics.h"
#include "classicminimaps.h"
#include "functions.h"
#include "mapInterface.h"
#include "classicminiPathfinding.h"

int main(void){
	if (!classicminigraphics::begin(1600, 900, "Maps", false)) {
		return -1;
	}

	texture::begin();
	classicminimaps::begin();
	mapInterface::begin();
	text::begin(200);

	thread newObject(specific::startPostcodeRoute);
	while (!glfwWindowShouldClose(classicminigraphics::window)) {
		classicminigraphics::startEvents();

		classicminimaps::mainloop();
		mapInterface::mainloop();
		specific::mainloop();

		classicminigraphics::endEvents();
	}
	glfwTerminate();
}