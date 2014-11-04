#include "Entity.h"	
#include <math.h>
#include <SDL_opengl.h>
#include <sstream>
#include<Windows.h>
using namespace std;

#define PI 3.1415926f


Entity::Entity(){}
Entity::Entity(float x, float y) : x(x) , y(y) {
	matrix.identity();
	width = 1;
	height = 1;
	z = 0;
	scale_x = 0.15;
	scale_y = 0.125;
	rotation = 0;
	speed = 0.1;
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

	Vector v( 0,speed, 0);
	/*Matrix m;
	m.identity();
	m.m[0][0] = 4; m.m[0][1] = 6; m.m[0][2] = 3;
	m.m[1][0] = 8; m.m[1][1] = 2; m.m[1][2] = 5;
	m.m[2][0] = 2; m.m[2][1] = 9; m.m[2][2] = 5;
	Matrix b;
	b.identity();
	b.m[0][0] = 4; b.m[0][1] = 6; b.m[0][2] = 3;
	b.m[1][0] = 8; b.m[1][1] = 2; b.m[1][2] = 5;
	b.m[2][0] = 2; b.m[2][1] = 9; b.m[2][2] = 5;
	Matrix c = m * b;*/
	
	Vector worldVeocity = matrix.inverse() * v;

	x += worldVeocity.x;
	y += worldVeocity.y;

	translate.m[0][3] = x;
	translate.m[1][3] = y;
	translate.m[2][3] = z;
	
	
	std::stringstream s;
	matrix = (translate * rotate)  ;
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j){
			s << matrix.m[i][j] << ' ';
		}
		s << std::endl;
	}
	/*s << std::endl;
	s << matrix.m[0][3] << std::endl;
	s << matrix.m[1][3] << std::endl;
	s << matrix.m[2][3] << std::endl;*/
	//s << v.x << endl;
	//s << v.y << endl;
	//s << v.z << endl;
	//s << endl;
	OutputDebugString(s.str().c_str());
}

void Entity::render() {
	buildMatrix();
	glPushMatrix();
	matrix.transpose(); 
	glMultMatrixf(matrix.ml);
	GLfloat quad[] = { -width / 2, height / 2,
		-width / 2, -height / 2,
		width / 2, -height / 2,
		width / 2, height / 2
	};
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINE_LOOP,0,4);
	glPopMatrix();
}
