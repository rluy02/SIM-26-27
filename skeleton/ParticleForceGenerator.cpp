#include "ParticleForceGenerator.h"
#include "ParticleWithForces.h"

void ParticleForceGenerator::updateForce(ParticleWithForces* p, float dt) {
	//1. Limpiar acumulador: ~f ← ~0
	//Calcular y acumular fuerzas : ~f ← ~f + F~i
	//3. Integrar estado : ~a ← ~f · w
	
	//p.setAcc()

	p->integrateVerlet(dt); //4. Actualizar ~v y p~con ∆t.
}