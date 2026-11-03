#pragma once
#include "ParticleWithForces.h"
class ProjectileWithForces : public ParticleWithForces
{
public:
	explicit ProjectileWithForces(const Vector3D& pos, const Vector3D& vel, const float mass = 0.0075f, const float damping = 1.00f,
		const float pSize = 2.f, const Vector4& color = Vector4(0.f, 1.f, 1.f, 1.f));
};

