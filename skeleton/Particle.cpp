#include "Particle.h"

Particle::Particle(const Vector3D& pos, const Vector3D& vel)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1.0f)); //referencia compartida
	_pos = physx::PxTransform(pos);
	_vel = vel;
	_renderItem = new RenderItem(shape, &_pos, Vector4(0.f, 1.f, 1.f, 1.f));
	shape->release();
}

Particle::~Particle()
{
	_renderItem->release();	_renderItem = nullptr;
}

void Particle::integrate(double t)
{
	explicitEuler(t);
}
void Particle::explicitEuler(double t)
{
	// xi + t · vi
	// vi + t · ai
	_pos.p = _pos.p + _vel * t;// No hace falta actualizar la velocidad porque es constante (a = 0)

}
void Particle::semiExplicitEuler(double t) {}
void Particle::verlet(double t) {}