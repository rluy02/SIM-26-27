#include "Projectile.h"

Projectile::Projectile(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, bool shootFromCamera, const double mass, const double damping)
	: Particle(pos, vel, acc, mass, damping), _shootFromCamera(shootFromCamera) {
}

void  Projectile::modifyVelocity(double valAccAdded) {
	//auto v = _vel.magnitude();
	//_vel += valAccAdded;
};