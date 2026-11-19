#include "SpringForceGenerator.h"
#include "../ParticleWithForces.h"

SpringForceGenerator::SpringForceGenerator(float k, float resting_length, ParticleWithForces* p) : _k(k),
_restingLength(resting_length), _otherP(p) {
}

void SpringForceGenerator::updateForce(ParticleWithForces* p, float dt) {
	//Aplicamos la fuerza a la particula "p"
	Vector3D dir = _otherP->getPosition() - p->getPosition(); //IMP que la particula no este muy cerca del ancla, puede dar error
	if (dir.dot(dir) < 1e-6f) return; //es nulo, extremos coincidentes.

	//Longitud - Longitud en reposo
	const float deltaX = dir.normalized() - _restingLength;
	const Vector3D force = _k * deltaX * dir; //relativePosVec se normaliza internamente en la llamada anterior
	p->addForce(force);
}

//Ley de Hooke
//F = -k (L - L0) d 
//d:direccion normalizada.

//El signo de k depende de que apunta a que. En este caso, p hacia _otherP,
//asi que la fuerza del muelle sobre p debe apuntar hacia _otherP cuando está estirado.