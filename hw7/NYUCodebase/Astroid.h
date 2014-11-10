#pragma once
#include "Entity.h"
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6
#define ROCKS 10


class Astroid {
public:
	Astroid();
	~Astroid();
	std::vector<Entity*> entities;
	void checkCollision();
	void init();
	bool updateAndRender();
	void fixedUpdate();
	void render();
	int count = 0;
	void update(float elapsed);
	Entity player;
	void genEntities();
	void wrapEntities();
	float lastFrameTicks;
	float timeLeftOver;
	GLuint texture;
	SDL_Window* displayWindow;

};

