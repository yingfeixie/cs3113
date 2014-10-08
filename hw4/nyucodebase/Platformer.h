#pragma once

#include <vector>

#include "Entity.h"
#define MAX_ENEMY 8


class Platformer {
public:
	Platformer();
	~Platformer();
	std::vector<Entity> entities;
	std::vector<Entity> blocks;
	void checkCollision(Entity& entity);
	void init();
	bool updateAndRender();
	void fixedUpdate();
	void render();
	void update(float elapsed);

	int enemyindex;
	float spawncount;

	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;

};

