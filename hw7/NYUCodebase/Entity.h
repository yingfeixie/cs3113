#pragma once
#include "Matrix.h"

class Entity {
public:
	Entity::Entity();
	Entity::Entity(float x, float y);

	Matrix matrix;
	float x;
	float y;
	float z;
	float scale_x;
	float scale_y;
	float width;
	float height;	
	float rotation;
	float speed;
	float velocity_x;
	float velocity_y;
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

	bool collidesWith(Entity *entity);
	void fixedUpdate();
	void buildMatrix();
	void render();

};