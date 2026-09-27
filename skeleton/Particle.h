#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
class Particle
{
public:
	Particle(const Vector3D& pos, const Vector3D& vel);
	~Particle();

	void integrate(double t);
private:
	void explicitEuler(double t);
	//Opcional
	void semiExplicitEuler(double t);
	void verlet(double t);
private:

	Vector3D _vel;
	physx::PxTransform _pos;
	RenderItem* _renderItem;
};

/*Notas: Mantener referencias para poder avanzar en step las fisicas (se instancia en el init lo basico)*/
/*P1.1 para la semana que viene*/
/*Aplicar velocidad constante, pero en prox practs solo la velocidad puede ser modificada por la aceleracion y la aceleracion por la fuerza*/


