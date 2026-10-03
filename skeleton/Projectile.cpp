#include "Projectile.h"

Projectile::Projectile(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, const double mass, const double damping,
	const float pSize, const Vector4& color) : Particle(pos, vel, acc, mass, damping, pSize, color) {
}