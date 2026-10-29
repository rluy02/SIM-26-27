#include "GravityGenerator.h"
#include "ParticleWithForces.h"

GravityGenerator::GravityGenerator(const Vector3D& gravity)
	: _gravity(gravity) {
}

void GravityGenerator::updateForce(ParticleWithForces* p, float dt) {
	assert(p != nullptr);
	Vector3D force = p->getMass() * _gravity; //m*g
	p->addForce(force); //aplicar el calculado
}