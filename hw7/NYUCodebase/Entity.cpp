#include "Entity.h"	
#include <math.h>
#include <SDL_opengl.h>

#define PI 3.1415926f


Entity::Entity(){}
Entity::Entity(float x, float y) : x(x) , y(y) {
	matrix.identity();
	width = 1;
	height = 1;
	z = 0;
	scale_x = 1;
	scale_y = 1;
	rotation = 0;
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
	matrix =  scale * rotate * translate;
}

void Entity::render() {
	buildMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(matrix.ml);
	//darw
	
	GLfloat quad[] = { -width / 2, height / 2,
		-width / 2, -height / 2,
		width / 2, -height / 2,
		width / 2, height / 2
	};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_QUADS,0,4);
	glPopMatrix();
}
