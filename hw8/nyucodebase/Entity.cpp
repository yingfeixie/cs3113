#include "Entity.h"	
#include <math.h>
#include <SDL_opengl.h>
#include <sstream>
#include<Windows.h>
#include <algorithm>
using namespace std;

#define PI 3.1415926f


Entity::Entity(){}
Entity::Entity(float x, float y, float z) : x(x) , y(y), z(z) {
	matrix.identity();
	width = 1;
	height = 1;
	z = 0;
	scale_x = 1;
	scale_y = 1;
	rotation = 0;
	speed = 0;
	collided = false;
	buildMatrix();
}

void Entity::buildMatrix(){
	Matrix rotate;
	rotate.identity();
	rotate.m[0][0] = cos(rotation/180*PI);
	rotate.m[0][1] = -sin(rotation / 180 * PI);
	rotate.m[1][0] = sin(rotation / 180 * PI);
	rotate.m[1][1] = cos(rotation / 180 * PI);

	Matrix scale;
	scale.identity();
	scale.m[0][0] = scale_x;
	scale.m[1][1] = scale_y;

	Matrix translate;
	translate.identity();
	translate.m[0][3] = x;
	translate.m[1][3] = y;
	translate.m[2][3] = z;
	
	matrix = (translate * rotate) * scale ;

	Vector worldVeocity = matrix* velocity;
	x = worldVeocity.x;
	y = worldVeocity.y;
}

bool Entity::checkCollisionX(Entity* entity){
	buildMatrix();
	entity->buildMatrix();
	Vector topLeft = entity->matrix * Vector(-entity->width / 2, entity->height / 2, 0);
	Vector topRight = entity->matrix * Vector(entity->width / 2, entity->height / 2, 0);
	Vector BottomLeft = entity->matrix * Vector(-entity->width / 2, -entity->height / 2, 0);
	Vector BottomRight = entity->matrix * Vector(entity->width / 2, -entity->height / 2, 0);

	topLeft = matrix.inverse()* topLeft;
	topRight = matrix.inverse()* topRight;
	BottomLeft = matrix.inverse()* BottomLeft;
	BottomRight = matrix.inverse()* BottomRight;

	float xs[] = { topLeft.x, topRight.x, BottomLeft.x, BottomRight.x };
	float min = *min_element(xs, xs+4);
	float max = *max_element(xs, xs + 4);
	
	if (min <= width / 2 && max >= -width / 2) {
		return true;
	}
	else return false;
}


bool Entity::checkCollisionY(Entity* entity){
	Vector topLeft = entity->matrix * Vector(-entity->width / 2, entity->height / 2, 0);
	Vector topRight = entity->matrix * Vector(entity->width / 2, entity->height / 2, 0);
	Vector BottomLeft = entity->matrix * Vector(-entity->width / 2, -entity->height / 2, 0);
	Vector BottomRight = entity->matrix * Vector(entity->width / 2, -entity->height / 2, 0);

	topLeft = matrix.inverse() * topLeft;
	topRight = matrix.inverse() * topRight;
	BottomLeft = matrix.inverse()  * BottomLeft;
	BottomRight = matrix.inverse()  * BottomRight;

	float ys[] = { topLeft.y, topRight.y, BottomLeft.y, BottomRight.y };
	float min = *min_element(ys, ys + 4);
	float max = *max_element(ys, ys + 4);
	stringstream s;
	if (min <= height / 2 && max >= -height / 2) {
		return true;
	}
	else return false;
}

bool Entity::collidesWith(Entity* anotherEntity){
	return (checkCollisionX(anotherEntity) && checkCollisionY(anotherEntity));
}
void Entity::render() {
	glPushMatrix();
	Matrix c = matrix;
	c.transpose(); 
	glMultMatrixf(c.ml);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLfloat quad[] = { -width / 2, height / 2,
		-width / 2, -height / 2,
		width / 2, -height / 2,
		width / 2, height / 2
	};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS,0,4);
	glPopMatrix();
}
