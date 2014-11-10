#include "Astroid.h"
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>
#include <algorithm>
using namespace std;


GLuint loadTexture(const char* imagePath) {
	SDL_Surface* surface = IMG_Load(imagePath);
	GLuint textId;
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(surface);
	return textId;
}


Astroid::Astroid()  {
	init();
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;

}


Astroid::~Astroid() {
	for (int i = 0; i < entities.size(); ++i) {
		delete entities[i];
	}
	SDL_Quit();
}

void Astroid::genEntities(){
	Entity* player = new Entity(0, 0, 0);
	player->isPlayer = true;
	player->texture = texture;
	player->width = 99.0f / 1024.0f;
	player->height = 75.0f / 1024.0f;
	player->u = 211.0f / 1024.0f;
	player->v = 941.0f / 1024.0f;
	entities.push_back(player);
	for (int i = 0; i < ROCKS; ++i) {
		float x = (float)rand() / RAND_MAX ;
		float y = (float)rand() / RAND_MAX;

		float rotation =  rand() % 45;
		Vector velocity (0.0001* x , 0.0001* y, 0);
		Entity* rock = new Entity(x, y, 0);
		rock->velocity = velocity;
		rock->rotation = rotation;
		rock->scale_x = 3;
		rock->scale_y = 3;
		rock->texture = texture;
		rock->width = 101.0f / 1024.0f;
		rock->height = 84.0f / 1024.0f;
		rock->u = 224.0f / 1024.0f;
		rock->v = 664.0f / 1024.0f;
		entities.push_back(rock);
	}
	
	
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
	texture = loadTexture("sheet.png");
	genEntities();	

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
void Astroid::checkCollision(){

	stringstream s;
	for (int i = 1; i < entities.size(); ++i){
		if (entities[0]->collidesWith(entities[i]) && entities[i]->collidesWith(entities[0])) {
			entities[0]->velocity.y = 0;
			float xdifference = entities[0]->x - entities[i]->x;
			float ydifference = entities[0]->y - entities[i]->y;

			Vector normalDiff(xdifference, ydifference, 0);
			normalDiff.normalize();
			entities[0]->x += 0.01*normalDiff.x;
			entities[0]->y += 0.01*normalDiff.y;
		}
	}
	
	for (int i = 1; i < entities.size(); ++i){
		for (int j = 1; j < entities.size(); ++j){
			if (entities[i] != entities[j])	{
				if (entities[i]->collidesWith(entities[j]) && entities[j]->collidesWith(entities[i])) {
					float xdifference = entities[i]->x - entities[j]->x;
					float ydifference = entities[i]->y - entities[j]->y;
					Vector normalDiff(xdifference, ydifference, 0);
					normalDiff.normalize();
					Vector reverseNormalDiff(-xdifference, -ydifference, 0);
					entities[i]->x += 0.01*normalDiff.x;
					entities[i]->y += 0.01*normalDiff.y;
					entities[j]->x += 0.01*reverseNormalDiff.x;
					entities[j]->y += 0.01*reverseNormalDiff.y;
				}
			}
		}
	}
}

void Astroid::wrapEntities(){
	for (int i = 0; i < entities.size(); ++i){
		if (entities[i]->x > 1.5 && entities[i]->velocity.x > 0 || entities[i]->x > 3) entities[i]->x = -1.5;
		else if (entities[i]->x < 1.5&& entities[i]->velocity.x < 0 || entities[i]->x < -3)entities[i]->x = 1.5;
		if (entities[i]->y > 1.1&& entities[i]->velocity.y > 0 || entities[i]->y >3)entities[i]->y = -1.1;
		else if (entities[i]->y > 1.1&& entities[i]->velocity.y < 0 || entities[i]->y < -3)entities[i]->y = 1.1;
		
	}
}
void Astroid::fixedUpdate() {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) entities[0]->rotation += FIXED_TIMESTEP * 80;
	else if (keys[SDL_SCANCODE_RIGHT]) entities[0]->rotation -= FIXED_TIMESTEP * 80;
	if (keys[SDL_SCANCODE_UP]) entities[0]->velocity.y = 0.001;
	else if (keys[SDL_SCANCODE_DOWN]) entities[0]->velocity.y = -0.001;
	else entities[0]->velocity.y = 0;
	checkCollision();
	for (int i = 1; i < entities.size(); ++i){
		entities[i]->rotation += 0.001*entities[i]->rotation;
	}
	wrapEntities();
}


void Astroid::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (Entity* e : entities) {
		e->render();
	}
	SDL_GL_SwapWindow(displayWindow);
}
