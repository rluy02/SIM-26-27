#include "Particle.h"
#include "SimulationConfig.h"
#include <iostream>

Particle::Particle(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, const float mass, const float damping, const float pSize, const Vector4& color)
	: _vel(vel), _acc(acc), _mass(mass), _damping(damping)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(pSize));//referencia compartida
	_pos = physx::PxTransform(pos);
	const float dt = simulation::fixedTimestepF;
	//x(t - dt) = x(t) - v(t)dt + 0.5 *a(t)dt^2   ... pos-1(-dt) desarrollo de taylor de orden 2
	_prevPos = pos - vel * dt + 0.5f * _acc * dt * dt;
	_dampingFactor = static_cast<float>(std::pow(_damping, dt));
	_renderItem = new RenderItem(shape, &_pos, color);
	shape->release();
}

Particle::~Particle()
{
	_renderItem->release();	_renderItem = nullptr;
}

void Particle::integrateVerlet(double t)
{
	assert(simulation::fixedTimestepF == t);
	const float dt = static_cast<float>(t);
	verlet(dt);
}
void Particle::integrateSEE(double t)
{
	assert(simulation::fixedTimestepF == t);
	const float dt = static_cast<float>(t);
	semiExplicitEuler(dt);
}
void Particle::explicitEuler(float dt)
{
	// xi + h · vi
	// vi + h · ai
	_pos.p = _pos.p + dt * _vel;
	_vel = (_vel + dt * _acc) * _dampingFactor;

}
void Particle::semiExplicitEuler(float dt) //Actualiza velocidad antes que la posicion (más preciso y estable)
{
	//vi + 1 = vi + h · ai
	//xi + 1 = xi + h · vi + 1
	_vel = (_vel + dt * _acc) * _dampingFactor; // damping(d ^ dt)
	_pos.p = _pos.p + dt * _vel;

}
void Particle::verlet(float dt)
{
	const Vector3D currentPos = _pos.p;
	const Vector3D displacement = currentPos - _prevPos;
	const Vector3D nextPos = currentPos + displacement * _dampingFactor + _acc * (dt * dt);

	_prevPos = currentPos; // La actual pasa a ser la anterior
	_pos.p = nextPos;
}

void Particle::showParamsDebug() {
	std::cout << "Posicion: " << Vector3D(_pos.p) << std::endl;
	std::cout << "Velocidad: " << _vel << std::endl;
	std::cout << "Aceleracion: " << _acc << std::endl;
	std::cout << "Masa: " << _mass << std::endl;
	std::cout << "Dumping: " << _damping << std::endl;

}

/*Nota.
A un damping de 0.90 significa que la particula pierde un 10% de su velocidad cada segundo.
Es decir, la velocidad aumenta/disminuye hasta que el damping quita la misma cantidad de valor que añade la aceleracion, que es cuando se "capa" (velocidad terminal)
*/