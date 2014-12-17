#pragma once
#include"MatrixEntities.h"

using namespace std;

class Particle {
public:
	Vector position;
	Vector velocity;
	Vector velocityDeviation;
	float lifetime;
	float sizeDeviation; 

	Color colorDeviation;
};

class ParticleEmitter {
public:
	ParticleEmitter(unsigned int particleCount); 
	ParticleEmitter();
	~ParticleEmitter();


	void Update(float elapsed);
	void Render();
	Vector position;
	Vector gravity;
	float maxLifetime;

	Vector velocity;
	Vector velocityDeviation;

	Color startColor;
	Color endColor;
	Color colorDeviation;

	vector<Particle> particles;
};

