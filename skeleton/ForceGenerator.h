#pragma once
#include "Vector3D.h"

class ParticleWithForces;
class ForceGenerator {
public:
	virtual ~ForceGenerator() {};
protected:
	virtual void updateForce(ParticleWithForces* particle, float dt) = 0;
	Vector3D _force;
};

//Respecto al viento., se usa k1 o k2:
/* F=k1()+k2()
* k1= laminar
* k2= turbulento
*
*/
//Torbellino: xc,yc,zc
//Explosion: K/r^2(X-X.)*e^./.

/* Masa para todas las particulas
* Fuerzas son instantaneas (se aplican al instante)
* Fuerzas se suman (son vectores)
* Particulas deben de tener fuerzas diferentes
* Generadores deben tener area de efecto
* (opcional) Sist. de particulas tiene registro de fuerzas
* Fuerzas pueden afectar a unos sistemas y a otros no
*/