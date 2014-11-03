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

	void buildMatrix();
	void render();

};