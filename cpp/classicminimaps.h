#pragma once

#ifndef CLASSICMINIMAPS_H
#define CLASSICMINIMAPS_H

namespace classicminimaps {
	void begin();
	void mainloop();

	void loadChunks();
	void render();

	extern float scaleDivider;
	extern float height;
	extern int shaderProgram;
}

#endif 

