#include"ParticleSystem.h"

float lerp1(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void ParticleEmitter::Render(){
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> particleVertices;

	for (unsigned int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}

	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	vector<float> particleColors;
	for (unsigned int i = 0; i < particles.size(); i++) {
		float m = (particles[i].lifetime / maxLifetime);
		particleColors.push_back(lerp1(startColor.r, endColor.r, m) + particles[i].colorDeviation.r);
		particleColors.push_back(lerp1(startColor.g, endColor.g, m) + particles[i].colorDeviation.g);
		particleColors.push_back(lerp1(startColor.b, endColor.b, m) + particles[i].colorDeviation.b);
		particleColors.push_back(lerp1(startColor.a, endColor.a, m) + particles[i].colorDeviation.a);
	}
	glColorPointer(4, GL_FLOAT, 0, particleColors.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_BLEND);
	glPopMatrix();
};

ParticleEmitter::ParticleEmitter(){};

ParticleEmitter::ParticleEmitter(unsigned int particleCount){
	maxLifetime = 2;

	position.z = 0;

	velocity.x = 0;
	velocity.y = -1;
	velocityDeviation.x = 2;
	velocityDeviation.y = .5;

	velocity.x = 0;
	velocity.y = -.5;
	velocityDeviation.x = .25;
	velocityDeviation.y = .25;

	startColor.r = 1.0;
	startColor.g = 1.0;
	startColor.b = 0.0;
	startColor.a = 1.0;
	endColor.r = 1.0;
	endColor.g = 0;
	endColor.b = 0.0;
	endColor.a = 1.0;
	colorDeviation.r = 1;
	colorDeviation.g = 1;
	colorDeviation.b = 0;
	colorDeviation.a = 0;

	gravity.x = 0;
	gravity.y = -.5;


	for (int i = 0; i < particleCount; i++){
		Particle temp;
		temp.position.x = position.x;
		temp.position.y = position.y;
		temp.position.z = position.z;
		temp.velocityDeviation.x = -(velocityDeviation.x*.5) + (velocityDeviation.x*RANDOM_NUMBER);
		temp.velocityDeviation.y = -(velocityDeviation.y*.5) + (velocityDeviation.y*RANDOM_NUMBER);
		temp.velocity.x = (velocity.x) + (temp.velocityDeviation.x);
		temp.velocity.y = (velocity.y) + (temp.velocityDeviation.y);
		temp.colorDeviation.r = (-colorDeviation.r * 0.5) + (colorDeviation.r *RANDOM_NUMBER);
		temp.colorDeviation.b = (-colorDeviation.g * 0.5) + (colorDeviation.g *RANDOM_NUMBER);
		temp.colorDeviation.g = (-colorDeviation.b * 0.5) + (colorDeviation.b *RANDOM_NUMBER);
		temp.colorDeviation.a = (-colorDeviation.a * 0.5) + (colorDeviation.a *RANDOM_NUMBER);
		temp.lifetime = RANDOM_NUMBER/2;
		temp.sizeDeviation = .25;
		particles.push_back(temp);
	}
}

ParticleEmitter::~ParticleEmitter(){};

void ParticleEmitter::Update(float elapsed){
	for (unsigned int i = 0; i < particles.size(); i++){
		particles[i].lifetime += elapsed;
		particles[i].position.x += particles[i].velocity.x*elapsed;
		particles[i].velocity.x += gravity.x*elapsed;
		particles[i].position.y += particles[i].velocity.y*elapsed;
		particles[i].velocity.y += gravity.y*elapsed;
		if (particles[i].lifetime>=maxLifetime){
			particles[i].position.x = position.x;
			particles[i].position.y = position.y;
			particles[i].velocity.x = velocity.x + (particles[i].velocityDeviation.x);
			particles[i].velocity.y = velocity.y + (particles[i].velocityDeviation.y);
			particles[i].lifetime = RANDOM_NUMBER;
		}
	}
	
}

