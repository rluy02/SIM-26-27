#pragma once
#include "ForceGenerator.h"
#include "SimulationConfig.h"

class GravityGenerator :public ForceGenerator {
public:
	explicit GravityGenerator(const Vector3D& gravity = Vector3D(0.f, simulation::worldGravityY, 0.f));
	void updateForce(ParticleWithForces* p, float dt) override;
private:
	Vector3D _gravity;
};