#include "ParticleWithForces.h"
#include "SimulationConfig.h"
#include <cfloat>
#include <iostream>


ParticleWithForces::ParticleWithForces(const Vector3D& pos, const Vector3D& vel, const float mass, const float damping, const float pSize, const Vector4& color)
	: _vel(vel), _f(Vector3D(0.f, 0.f, 0.f)), _mass(mass), _damping(damping)
{
	assert(_mass > (1e-7f - FLT_EPSILON)); //basicamente que sea mayor que un 0 muy pequeño
	_invMass = 1.f / _mass;
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(pSize));//referencia compartida
	_pos = physx::PxTransform(pos);
	const float dt = simulation::fixedTimestepF;

	const Vector3D acc = _f * _invMass;
	//x(t - dt) = x(t) - v(t)dt + 0.5 *a(t)dt^2   ... pos-1(-dt) desarrollo de taylor de orden 2
	_prevPos = pos - vel * dt + 0.5f * acc * dt * dt;
	_dampingFactor = static_cast<float>(std::pow(_damping, dt));
	_renderItem = new RenderItem(shape, &_pos, color);
	shape->release();
}

ParticleWithForces::~ParticleWithForces()
{
	_renderItem->release();	_renderItem = nullptr;
}

void ParticleWithForces::integrateVerlet(double t)
{
	assert(simulation::fixedTimestepF == t);
	const float dt = static_cast<float>(t);
	verlet(dt);
}
void ParticleWithForces::integrateSEE(double t)
{
	assert(simulation::fixedTimestepF == t);
	const float dt = static_cast<float>(t);
	semiExplicitEuler(dt);
}

void ParticleWithForces::semiExplicitEuler(float dt) //Actualiza velocidad antes que la posicion (más preciso y estable)
{
	const Vector3D acc = _f * _invMass;
	//vi + 1 = vi + h · ai
	//xi + 1 = xi + h · vi + 1
	_vel = (_vel + dt * acc) * _dampingFactor; // damping(d ^ dt)
	_pos.p = _pos.p + dt * _vel;

}
void ParticleWithForces::verlet(float dt)
{
	const Vector3D acc = _f * _invMass;
	const Vector3D currentPos = _pos.p;
	const Vector3D displacement = currentPos - _prevPos;
	const Vector3D nextPos = currentPos + displacement * _dampingFactor + acc * (dt * dt);

	_prevPos = currentPos; // La actual pasa a ser la anterior
	_pos.p = nextPos;
}

void ParticleWithForces::clearForces() {
	_f = Vector3D(0.f, 0.f, 0.f);
}

void ParticleWithForces::addForce(const Vector3D& force) {
	_f += force; //agregar a la acumulada que pueda tener en el momento
}
void ParticleWithForces::showParamsDebug() {
	std::cout << "Posicion: " << Vector3D(_pos.p) << std::endl;
	std::cout << "Velocidad: " << _vel << std::endl;
	std::cout << "Fuerzas: " << _f << std::endl;
	std::cout << "Masa: " << _mass << std::endl;
	std::cout << "Dumping: " << _damping << std::endl;
}