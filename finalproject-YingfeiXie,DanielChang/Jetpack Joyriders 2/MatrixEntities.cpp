#include"MatrixEntities.h"

using namespace std;

Matrix::Matrix(){
	for (int i = 0; i < 16; i++){
		ml[i] = 0;
	}
};

Matrix Matrix::inverse() {
	float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
	float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
	float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
	float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];
	float v0 = m20 * m31 - m21 * m30;
	float v1 = m20 * m32 - m22 * m30;
	float v2 = m20 * m33 - m23 * m30;
	float v3 = m21 * m32 - m22 * m31;
	float v4 = m21 * m33 - m23 * m31;
	float v5 = m22 * m33 - m23 * m32;
	float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
	float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
	float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
	float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);
	float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
	float d00 = t00 * invDet;
	float d10 = t10 * invDet;
	float d20 = t20 * invDet;
	float d30 = t30 * invDet;
	float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;
	v0 = m10 * m31 - m11 * m30;
	v1 = m10 * m32 - m12 * m30;
	v2 = m10 * m33 - m13 * m30;
	v3 = m11 * m32 - m12 * m31;
	v4 = m11 * m33 - m13 * m31;
	v5 = m12 * m33 - m13 * m32;
	float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;
	v0 = m21 * m10 - m20 * m11;
	v1 = m22 * m10 - m20 * m12;
	v2 = m23 * m10 - m20 * m13;
	v3 = m22 * m11 - m21 * m12;
	v4 = m23 * m11 - m21 * m13;
	v5 = m23 * m12 - m22 * m13;
	float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
	float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
	float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
	float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;
	Matrix m2;
	m2.m[0][0] = d00;
	m2.m[0][1] = d01;
	m2.m[0][2] = d02;
	m2.m[0][3] = d03;
	m2.m[1][0] = d10;
	m2.m[1][1] = d11;
	m2.m[1][2] = d12;
	m2.m[1][3] = d13;
	m2.m[2][0] = d20;
	m2.m[2][1] = d21;
	m2.m[2][2] = d22;
	m2.m[2][3] = d23;
	m2.m[3][0] = d30;
	m2.m[3][1] = d31;
	m2.m[3][2] = d32;
	m2.m[3][3] = d33;
	return m2;
};

Vector::Vector(float x, float y, float z){
	x = x;
	y = y;
	z = z;
};

Vector::Vector(){
	x = 0;
	y = 0;
	z = 0;
};

float Vector::length(){
	return(sqrt(x*x + y*y + z*z));
};

void Vector::normalize(){
	float len = this->length();
	x /= len;
	y /= len;
	z /= len;
}

void Vector::dispV(){
	cout << "Vector Values: " << endl;
	cout << "X = " << x << endl;
	cout << "Y = " << y << endl;
	cout << "Z = " << z << endl;
	cout << endl;
	return;
};

void Matrix::identity(){
	for (int i = 0; i < 16; i++){
		ml[i] = 0;
	}
	m[0][0] = 1;
	m[1][1] = 1;
	m[2][2] = 1;
	m[3][3] = 1;
};

void Matrix::dispM(){
	cout << "Matrix[row][column]: " << endl;
	cout << "R/C 0   1   2   3" << endl;
	for (int row = 0; row < 4; row++){
		cout << row << ": ";
		for (int col = 0; col < 4; col++){
			cout << "[" << m[row][col] << "] ";
		}
		cout << endl;
	}
	cout << endl;
};

Matrix Matrix::operator*(const Matrix& m2){
	Matrix product;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			for (int inner = 0; inner < 4; inner++) {
				product.m[col][row] += (m[col][inner] * m2.m[inner][row]);
			}

		}

	}
	return product;
};

Vector Matrix::operator*(const Vector &v){
	Vector product;
	product.x = m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z + m[3][0];
	product.y = m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z + m[3][1];
	product.z = m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z + m[3][2];

	return product;
};

Entity::Entity(){
	x = 0.0f;
	y = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation = 0.0f;
	visible = true;
};

void Entity::buildMatrix(){
	Matrix rotate;
	rotate.identity();
	rotate.m[0][0] = cos(rotation / 180 * PI);
	rotate.m[1][0] = -sin(rotation / 180 * PI);
	rotate.m[0][1] = sin(rotation / 180 * PI);
	rotate.m[1][1] = cos(rotation / 180 * PI);

	Matrix scale;
	scale.identity();
	scale.m[0][0] = scale_x;
	scale.m[1][1] = scale_y;

	Matrix translate;
	translate.identity();
	translate.m[3][0] = x;
	translate.m[3][1] = y;
	
	matrix = scale*rotate*translate;

};

void Entity::Render(){
	buildMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glMultMatrixf(matrix.ml);

	Draw();

	glPopMatrix();	
}

void Entity::Draw() {
	if (visible){
		glEnable(GL_TEXTURE_2D);//enable or disable server-side GL capabilities, in this case enables 2d textures.
		glBindTexture(GL_TEXTURE_2D, textureID);//binds texture to target. Binds an image to the texture map.

		float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
		float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
		float spriteWidth = 1.0f / (float)spriteCountX;
		float spriteHeight = 1.0f / (float)spriteCountY;

		GLfloat quadUVs[] = { u + 0.002f, v + 0.002f,
			u + 0.002f, v + (spriteHeight)-0.002f,
			u + spriteWidth - 0.002f, v + (spriteHeight)-0.002f,
			u + spriteWidth - 0.002f, v + 0.002f
		};

		GLfloat quad[] = { -width / 2, height / 2, -width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2 };//Defines a quad to place the image. REMEMBER COUNTER CLOCKWISE LISTING OF VERTICES
		glVertexPointer(2, GL_FLOAT, 0, quad);//Read 2, FLOAT VALUES, SKIP 0 values in case we put colors in the matrix, and quad is the pointer to the array.
		glEnableClientState(GL_VERTEX_ARRAY);//allows for server to access the vertex arrays and for clients to draw the arrays.
		glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);//Defines an array of texture coordinates 
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_BLEND);//Enable blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Alpha blending, basically removing the background of the quad.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glDrawArrays(GL_QUADS, 0, 4);//Drawing quads, starting from 0, and draw 4 vertices. 
		glDisable(GL_TEXTURE_2D);//Disable the texture since OpenGl won't use the same texture when redrawing other quads.
		glDisable(GL_BLEND);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

}

void iMatrixToVec(Entity A,Vector& C){
	Vector temp;
	temp = C;
	C = A.matrix.inverse()*temp;
}

bool Entity::checkCollision(Entity A){
	Vector tRight, tLeft, bRight, bLeft;
	
	tRight.x = A.x + A.width / 2;
	tRight.y = A.y + A.height / 2;
	iMatrixToVec(*this, tRight);

	tLeft.x = A.x - A.width / 2;
	tLeft.y = A.y + A.height / 2;
	iMatrixToVec(*this, tLeft);

	bRight.x = A.x + A.width / 2;
	bRight.y = A.y - A.height / 2;
	iMatrixToVec(*this, bRight);

	bLeft.x = A.x - A.width / 2;
	bLeft.y = A.y - A.height / 2;
	iMatrixToVec(*this, bLeft);

	float max_x = max(max(tRight.x, tLeft.x), max(bRight.x, bLeft.x));
	float max_y = max(max(tRight.y, tLeft.y), max(bRight.y, bLeft.y));
	float min_x = min(min(tRight.x, tLeft.x), min(bRight.x, bLeft.x));
	float min_y = min(min(tRight.y, tLeft.y), min(bRight.y, bLeft.y));
	if (!(((min_x <= (width / 2)) && (max_x >= -(width / 2))) && (((min_y <= (height / 2)) && (max_y >= -(height / 2)))))){
		return false;
	}
	return true;
	
}

void Entity::resetCollisions(){
	collideBot = false;
	collideLeft = false;
	collideRight = false;
	collideTop = false;
}