#include "Entity.h"
using namespace std;

Entity::Entity(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {
	friction_x = 2.0f;
	acceleration_x = 0.0;
	acceleration_y = -2.0f;
	mass = 1.0;
	collidedTop = false;
	collidedBottom = false;
	collidedLeft = false;
	collidedRight = false;
}


void Entity::render() {
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	GLfloat quad[] = { -width / 2, height / 2,
		-width / 2, -height / 2,
		width / 2, -height / 2,
		width / 2, height / 2 };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS, 0, 4);
}

float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}
void Entity::fixedUpdate() {
	collidedBottom = false;
	collidedTop = false;
	collidedLeft = false;
	collidedRight = false;
	velocity_x = lerp(velocity_x, 0.0f, FIXED_TIMESTEP * friction_x);
	velocity_x += acceleration_x * FIXED_TIMESTEP;
	velocity_y += acceleration_y * FIXED_TIMESTEP;
	y += velocity_y * FIXED_TIMESTEP;
}

bool Entity::collidesWith(Entity* entity) {
	collidedBottom = false;
	collidedTop = false;
	collidedLeft = false;
	collidedRight = false;

	collidedBottom = (((entity->left() <= right() && right() <= entity->right()) ||
		(entity->left() <= left() && left() <= entity->right()))
		&& bot() <= entity->top() && top() > entity->top());

	collidedTop = (((entity->left() <= right() && right() <= entity->right()) ||
		(entity->left() <= left() && left() <= entity->right()))
		&& top() >= entity->bot() && bot() < entity->bot());

	collidedLeft = (((entity->bot() <= bot() && bot() <= entity->top()) ||
		(entity->bot() <= top() && top() <= entity->top()))
		&& left() <= entity->right() && right() > entity->right());

	collidedRight = (((entity->bot() <= bot() && bot() <= entity->top()) ||
		(entity->bot() <= top() && top() <= entity->top()))
		&& right() >= entity->left() && left() < entity->left());


	
	return (collidedBottom || collidedTop || collidedLeft || collidedRight);
}

float Entity::top() {
	return y + height / 2;
}
float Entity::bot() {
	return y - height / 2;
}
float Entity::left() {
	return x - width / 2;
}
float Entity::right() {
	return x + width / 2;
}
