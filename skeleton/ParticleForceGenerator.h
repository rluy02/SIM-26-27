#pragma once
#include "ForceGenerator.h"

class ParticleWithForces;

class ParticleForceGenerator : public ForceGenerator {
public:
	void updateForce(ParticleWithForces* particle, float dt);
private:

};