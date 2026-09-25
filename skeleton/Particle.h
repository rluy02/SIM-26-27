#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
class Particle
{
public:
	Particle(Vector3D Pos, Vector3D);
	~Particle();

	void integrate(double t);
private:
	Vector3D vel;
	physx::PxTransform pose;
	RenderItem* renderItem;
};

/*Notas: Mantener referencias para poder avanzar en step las fisicas (se instancia en el init lo basico)*/
/*P1.1 para la semana que viene*/
/*Aplicar velocidad constante, pero en prox practs solo la velocidad puede ser modificada por la aceleracion y la aceleracion por la fuerza*/


