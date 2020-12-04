#include "classicminiGraphics.h"
#include "classicminimaps.h"
#include "functions.h"
#include "mapInterface.h"

int main(void){
	if (!classicminigraphics::begin(1280, 720, "Maps", false)) {
		return -1;
	}
	texture::begin();
	classicminimaps::begin();
	mapInterface::begin();
	text::begin(200);
	

	while (!glfwWindowShouldClose(classicminigraphics::window)) {
		classicminigraphics::startEvents();
		
		classicminimaps::mainloop();
		mapInterface::mainloop();
		
		classicminigraphics::endEvents();
	}
	glfwTerminate();
}