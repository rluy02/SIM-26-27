#include "ProjectileWithForces.h"

ProjectileWithForces::ProjectileWithForces(const Vector3D& pos, const Vector3D& vel, const float mass, const float damping,
	const float pSize, const Vector4& color) : ParticleWithForces(pos, vel, mass, damping, pSize, color) {
}