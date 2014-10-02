#include <Platformer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
using namespace std;


Entity::Entity(float x, float y, float width, float height)
		: x(x), y(y), width(width), height(height) {}

void Platformer::init() {
	SDL_Window* displayWindow;
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);

	//load platform

	for (int i = 0; i < PLATFORM_LENGTH; ++i) {
		Entity(-1.0f+i(0.5f),-0.8f,0.5f,0.5f)
	}
}