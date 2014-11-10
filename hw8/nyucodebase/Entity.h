#pragma once
#include "Matrix.h"
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity {
public:
	Entity::Entity();
	Entity::Entity(float x, float y,float z);
	bool checkCollisionX(Entity* entity);
	bool checkCollisionY(Entity* entity);
	Matrix matrix;
	float set_x;
	float set_y;
	float x;
	float y;
	float z;
	float ydisp;
	float scale_x;
	float scale_y;
	float width;
	float height;	
	float rotation;
	float speed;
	Vector velocity;
	float acceleration_x;
	float acceleration_y;
	float friction_x;
	float friction_y;
	float mass;
	bool isPlayer;
	bool enableCollisions;
	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;
	bool collided;
	bool collidesWith(Entity *entity);
	void fixedUpdate();
	void buildMatrix();
	void render();
	GLuint texture;
	float u;
	float v;
};
