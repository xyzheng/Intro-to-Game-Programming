#pragma once
#include "Particle.h"
#include <vector>

using namespace std;

class ParticleEmitter {
public:
    ParticleEmitter(unsigned int particleCount);

    ParticleEmitter();
    ~ParticleEmitter();

    void update(float elapsed);
    void render();
    
    Vector position;
    Vector velocity;
	Vector gravity;
    float velocityDeviation;
    float maxLifetime;
    vector<Particle> particles;
};