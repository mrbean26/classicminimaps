#include "classicminiGraphics.h"
#include "classicminimaps.h"
#include <iostream>
int main(void){
	classicminigraphics::begin(360, 640, "Maps");
	classicminimaps::begin();

	while (!glfwWindowShouldClose(classicminigraphics::window)) {
		classicminigraphics::startEvents();
		
		classicminimaps::mainloop();
		
		classicminigraphics::endEvents();
	}
	glfwTerminate();
}