#pragma once
#include "Entity.h"
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define ROCKS 4


class Astroid {
public:
	Astroid();
	~Astroid();
	std::vector<Entity*> entities;
	void checkCollision(Entity& entity);
	void init();
	bool updateAndRender();
	void fixedUpdate();
	void render();
	void update(float elapsed);
	Entity player;
	void genRocks();
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

};

