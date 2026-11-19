#include "AnchoredSpringFG.h"
#include "../ParticleWithForces.h"

AnchoredSpringFG::AnchoredSpringFG(float k, float restingLength, const Vector3D& anchorPos)
	: SpringForceGenerator(k, restingLength, nullptr)
{
	// Ancla: partícula estática (nunca se integra) con forma de cubo
	_otherP = new ParticleWithForces(anchorPos, Vector3D(0.f, 0.f, 0.f), 1e6f, 1.f, 0.5f,
		Vector4(1.f, 1.f, 0.f, 1.f), ParticleShape::Box);
}

AnchoredSpringFG::~AnchoredSpringFG() {
	delete _otherP;
}