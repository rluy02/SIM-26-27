#include "SpringForceGenerator.h"
#include "../ParticleWithForces.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_length, ParticleWithForces* p) : _k(k),
_restingLength(resting_length), _otherP(p) {
}

void SpringForceGenerator::updateForce(ParticleWithForces* p, float dt) {
	//Aplicamos la fuerza a la particula "p"
	Vector3D relativePosVec = _otherP->getPosition() - p->getPosition();

	//Longitud - Longitud en reposo
	const float deltaX = relativePosVec.normalized() - _restingLength;
	Vector3D force = _k * deltaX * relativePosVec; //relativePosVec se normaliza internamente en la llamada anterior
	p->addForce(force);
}

//Ley de Hooke
//F = -k (L - L0) d 
//d:direccion normalizada.

//El signo de k depende de que apunta a que. En este caso, p hacia _otherP,
//asi que la fuerza del muelle sobre p debe apuntar hacia _otherP cuando está estirado.