#include "Platformer.h"
#include <vector>
#include <string>
using namespace std;

Platformer::Platformer() : entities(MAX_ENEMY) {
	init();
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	enemyindex = 1;
	spawncount = 0;
}


Platformer::~Platformer() {
	SDL_Quit();
}

void Platformer::init() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33f, 1.33f, -1.0f, 1.0f, -1.0f, 1.0f);
	glClearColor( 0.5f,0.5f, 0.0f, 0.5f);
	glMatrixMode(GL_MODELVIEW);

	Entity player(0, -1 + 0.5 + 0.25, 0.5, 0.5);
	player.enableCollisions = true;
	player.isPlayer = true;
	player.velocity_x = 0;
	player.velocity_y = 0;
	entities[0] = (player);

	
	
	for (int i = -2; i < 3; ++i){
		float width = 0.5;
		float height = 0.25;
		float x = i*width;
		float y = -1.0 + height / 2;
		Entity block(x, y, width, height);
		block.enableCollisions = false;
		block.isPlayer = false;
		blocks.push_back(block);
	}
	for (int i = 0; i < 6; ++i){
		float width = 0.25;
		float height = 0.5;
		float x = -1.33;
		float y = -1.0 + i*height;
		Entity block(x, y, width, height);
		block.enableCollisions = false;
		block.isPlayer = false;
		blocks.push_back(block);
	}
	for (int i = 0; i < 6; ++i){
		float width = 0.25;
		float height = 0.5;
		float x = 1.33;
		float y = -1.0 + i*height;
		Entity block(x, y, width, height);
		block.enableCollisions = false;
		block.isPlayer = false;
		blocks.push_back(block);
	}

}


bool Platformer::updateAndRender() {
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
	//spawn 
	timeLeftOver = fixedElapsed;
	spawncount += elapsed;
	if (spawncount > 2.0f){
		Entity enemy(0, 1.5 + 0.25, 0.25, 0.25);
		enemy.enableCollisions = true;
		enemy.isPlayer = false;
		enemy.velocity_x = rand() % 2;
		enemy.velocity_y = 0;
		enemy.friction_x = 0;
		entities[enemyindex] = enemy;
		enemyindex++;
		spawncount = 0;
	}
	if (enemyindex > MAX_ENEMY-1) enemyindex = 1;
	render();
	return false;
}

void Platformer::fixedUpdate() {
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) entities.front().acceleration_x = -2.0f;
	else if (keys[SDL_SCANCODE_RIGHT]) entities.front().acceleration_x = 2.0f;
	else entities.front().acceleration_x = 0.0f;
	if (keys[SDL_SCANCODE_UP] && entities.front().collidedBottom) entities.front().velocity_y = 1.0f;


	for (Entity& e : entities) {//check y move y
		e.fixedUpdate();
		for (Entity& block : blocks){
			e.collidesWith(&block);
			float penetration = fabs(fabs(e.y - block.y) - e.height / 2 - block.height / 2);
			if (e.collidedBottom&& e.enableCollisions) {
				e.y += penetration + 0.001*e.height;
				e.velocity_y = 0;
				if (keys[SDL_SCANCODE_UP] && e.isPlayer) e.velocity_y = 1.0f;
				break;
			}
			else if (e.collidedTop) {
				e.y -= penetration + 0.001*e.height;
				e.velocity_y = 0;
				break;
			}
		}
	}

	for (Entity& e : entities) {//check x move x
		e.x += e.velocity_x * FIXED_TIMESTEP;
		for (Entity& block : blocks){
			e.collidesWith(&block);
			float penetration = fabs(fabs(e.x - block.x) - e.width / 2 - block.width / 2);
			if (e.collidedLeft) {
				e.x += penetration + 0.001*e.height;
				e.velocity_x = 0;
				break;
			}
			else if (e.collidedRight) {
				e.x -= penetration + 0.001*e.height;
				e.velocity_x = 0;
				break;
			}
		}
	}

	for (Entity& e : entities) {// check being stepped on by player
		e.collidesWith(&entities[0]);
		if (e.collidedTop) {
			e.enableCollisions = false;
		}
	}
	for (int i = 0; i < entities.size()-1; ++i){//check to pile up entities
		for (int j = 0; j < entities.size()-1; ++j){
			if (&entities[i] != &entities[j] && entities[i].collidesWith(&entities[j])) {
				if (entities[i].collidedTop){
					entities[j].velocity_y = 0;
					entities[j].y += fabs(fabs(entities[i].y - entities[j].y) - entities[i].height / 2 - entities[j].height / 2);
					break;
				}
			}

		}
	}
}


void Platformer::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (Entity& e : entities) {
		e.render();
	}
	for (Entity b : blocks) {
		b.render();
	}
	SDL_GL_SwapWindow(displayWindow);
}
