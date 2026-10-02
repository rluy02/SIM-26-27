#include "Particle.h"
#include "SimulationConfig.h"
#include <iostream>

Particle::Particle(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, const double mass, const double damping)
	: _vel(vel), _acc(acc), _mass(mass), _damping(damping)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(10.0f)); //referencia compartida
	_pos = physx::PxTransform(pos);
	const double dt = simulation::fixedTimestep;
	//x(t - dt) = x(t) - v(t)dt + 0.5 *a(t)dt^2   ... pos-1(-dt) desarrollo de taylor de orden 2
	_prevPos = (pos - vel) * dt + 0.5 * _acc * dt * dt;
	_renderItem = new RenderItem(shape, &_pos, Vector4(0.f, 1.f, 1.f, 1.f));
	shape->release();
}

Particle::~Particle()
{
	_renderItem->release();	_renderItem = nullptr;
}

void Particle::integrate(double t)
{
	verlet(t);
}
void Particle::explicitEuler(double t)
{
	// xi + h · vi
	// vi + h · ai
	_pos.p = _pos.p + t * _vel;
	const double dampingFactor = std::pow(_damping, t);
	_vel = (_vel + t * _acc) * dampingFactor;

}
void Particle::semiExplicitEuler(double t) //Actualiza velocidad antes que la posicion (más preciso y estable)
{
	//vi + 1 = vi + h · ai
	//xi + 1 = xi + h · vi + 1
	const double dampingFactor = std::pow(_damping, t);
	_vel = (_vel + t * _acc) * dampingFactor; // damping(d ^ dt)
	_pos.p = _pos.p + t * _vel;

}
void Particle::verlet(double t)
{
	Vector3D currentPos = _pos.p;
	Vector3D displacement = currentPos - _prevPos;
	const double dampingFactor = std::pow(_damping, t);
	Vector3D nextPos = currentPos + displacement * dampingFactor + _acc * (t * t);

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
A un damping de 0.90 significa un 10% de velocidad total (acumulada) que se pierde en cada segundo.
Es decir, la velocidad aumenta o disminuye hasta que el damping quita la misma cantidad de valor que añade la aceleracion, que es cuando se "capa"
*/