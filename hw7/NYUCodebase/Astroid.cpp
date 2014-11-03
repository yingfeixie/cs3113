#include "Astroid.h"
#include <vector>
#include <string>
using namespace std;

Astroid::Astroid()  {
	init();
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

}


Astroid::~Astroid() {
	SDL_Quit();
}

void Astroid::genRocks(){

}
void Astroid::init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	player.x = 0;
	player.y = 0;
	player.matrix.identity();
	player.width = 1;
	player.height = 1;
	player.z = 0;
	player.scale_x = 0.15;
	player.scale_y = 0.125;
	player.rotation = 0;
	player.speed =100;
	
}


bool Astroid::updateAndRender() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)	{
			return true;
		}
	}
	float ticks = SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP *MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP *MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		fixedUpdate();
	}

	timeLeftOver = fixedElapsed;

	render();
	return false;
}

void Astroid::fixedUpdate() {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) player.rotation += FIXED_TIMESTEP*player.speed;
	else if (keys[SDL_SCANCODE_RIGHT]) player.rotation -= FIXED_TIMESTEP*player.speed;
	if (keys[SDL_SCANCODE_UP]) player.y += FIXED_TIMESTEP;

}


void Astroid::render() {
	glClear(GL_COLOR_BUFFER_BIT);

	player.render();
	for (Entity* e : entities) {
		e->render();
	}
	SDL_GL_SwapWindow(displayWindow);
}
