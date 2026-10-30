#define _USE_MATH_DEFINES 
#include <cmath>
#include "WindGenerator.h"
#include "ParticleWithForces.h"


WindGenerator::WindGenerator(const Vector3D& vel, float radius, float densidad, float viscosidad, float coefArraste)
	: _vel(vel), _r(radius), _densidadAire(densidad), _viscosidadAire(viscosidad), _coefArraste(coefArraste) {

	_k1 = 6.f * static_cast<float>(M_PI) * _viscosidadAire * _r;
	//para k2
	float A = static_cast<float>(M_PI) * (_r * _r);
	_k2 = 0.5f * _coefArraste * _densidadAire * A;
}

void WindGenerator::updateForce(ParticleWithForces* p, float dt) {
	assert(p != nullptr);

	const Vector3D vRel = _vel - p->getVel(); //imp este orden
	const Vector3D fLineal = _k1 * vRel;

	//Calculo de newton
	const Vector3D fCuadratica = (_k2 * vRel.magnitude()) * vRel;

	const Vector3D force = fLineal + fCuadratica;
	p->addForce(force); //aplicar el calculado
}

//Fv = 𝑘1(V𝑣 − 𝑣) + 𝑘2‖Vv − 𝑣 ‖(V𝑣 − 𝑣)
//Todos son vectores menos las 'ks' que son constantes. Vv es la velocidad del viento
//La primer parte del termino es la fuerza lineal

//Al calcular k2, tener en cuenta el 0.5
//Fdrag =0.5*Cd ρ*A*v*v

//Para k1 SOLO con esferas, se hace uso de stokes k1 = 6*pi*viscosidad*r