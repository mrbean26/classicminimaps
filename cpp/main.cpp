#include "classicminiGraphics.h"
#include "classicminimaps.h"
#include "functions.h"

#include <iostream>

int main(void){
	if (!classicminigraphics::begin(360, 640, "Maps")) {
		return -1;
	}
	classicminimaps::begin();
	text::begin(200);

	while (!glfwWindowShouldClose(classicminigraphics::window)) {
		classicminigraphics::startEvents();
		
		classicminimaps::mainloop();
		
		classicminigraphics::endEvents();
	}
	glfwTerminate();
}