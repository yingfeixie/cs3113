#pragma once
//#include "Platformer.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

class Entity {
public:
	Entity(){}
	Entity(float x, float y, float width, float height);
	void update(float elapsed);
	void render();
	bool collidesWith(Entity *entity);
	void fixedUpdate();
//	SheetSprite sprite;
	float x;
	float y;
	float width;
	float height;

	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;

	float top();
	float bot();
	float left();
	float right();

	bool isPlayer;
	bool enableCollisions;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
};