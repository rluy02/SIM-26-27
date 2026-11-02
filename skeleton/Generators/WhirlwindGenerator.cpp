#include "WhirlwindGenerator.h"
#include "../ParticleWithForces.h"

WhirlwindGenerator::WhirlwindGenerator(const Vector3D& pos, float rad, float rInfl, float vs, float k) : WindGenerator(Vector3D(0.f, 0.f, 0.f), rad),
_pos(pos), _radiusInfluence(rInfl), _verticalStop(vs), _k(k) {
}


Vector3D WhirlwindGenerator::getWindVelocity(const ParticleWithForces* p) const {
	assert(p != nullptr);
	const Vector3D pPos = p->getPosition();

	const float dx = pPos.x - _pos.x;
	const float dy = _verticalStop - (pPos.y - _pos.y);
	const float dz = pPos.z - _pos.z;

	const float distSq = dx * dx + dz * dz;

	if (distSq > _radiusInfluence * _radiusInfluence) {
		return Vector3D(0.f, 0.f, 0.f);
	}

	return _k * Vector3D(-dz, dy, dx);
}