#include "Particle.h"
#include <iostream>

Particle::Particle(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, double damping)
	: _vel(vel), _acc(acc), _damping(damping)
{
	physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(10.0f)); //referencia compartida
	_pos = physx::PxTransform(pos);
	_prevPos = pos - vel * (1.0 / 60.0); // posi-1=pos0-vel0*dt  (esto es una estimacion)
	_renderItem = new RenderItem(shape, &_pos, Vector4(0.f, 1.f, 1.f, 1.f));
	shape->release();
}

Particle::~Particle()
{
	_renderItem->release();	_renderItem = nullptr;
}

void Particle::integrate(double t)
{
	semiExplicitEuler(t);
}
void Particle::explicitEuler(double t)
{
	// xi + h · vi
	// vi + h · ai
	_pos.p = _pos.p + t * _vel;
	_vel = (_vel + t * _acc) * std::pow(_damping, t);

}
void Particle::semiExplicitEuler(double t) //Actualiza velocidad antes que la posicion (más preciso y estable)
{
	//vi + 1 = vi + h · ai
	//xi + 1 = xi + h · vi + 1
	_vel = (_vel + t * _acc) * std::pow(_damping, t); // damping(d ^ dt)
	_pos.p = _pos.p + t * _vel;
	//std::cout << _vel << std::endl;

}
void Particle::verlet(double t) //Va mucho más rapido, falta revisar que sucede
{
	Vector3D currentPos = _pos.p;
	//xi + 1 = 2xi − xi−1 + h^2· ai
	Vector3D displacement = currentPos - _prevPos;
	Vector3D nextPos = _pos.p + displacement * std::pow(_damping, t) + (t * t) * _acc;
	
	_prevPos = currentPos; // La actual pasa a ser la anterior
	_pos.p = nextPos;
}

/*Nota.
A un damping de 0.90 significa un 10% de velocidad total (acumulada) que se pierde en cada fotograma.
Es decir, la velocidad aumenta o disminuye hasta que el damping quita la misma cantidad de valor que añade la aceleracion, que es cuando se "capa"
*/