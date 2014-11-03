
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "Matrix.h"
#include "Entity.h"
#include <iostream>
#include <sstream>
#include <Windows.h>

SDL_Window* displayWindow;
using namespace std;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	bool done = false;
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	Entity e(2, 3);
	stringstream s;
	e.render();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j){
			s << e.matrix.m[i][j];
		}s << endl;
	}
	s << endl;
	OutputDebugString(s.str().c_str());
	
	
	SDL_Event event;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);

		
		e.render();
		
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
	
}