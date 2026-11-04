#pragma once
#include "WindGenerator.h"

class WhirlwindGenerator : public WindGenerator {
public:
	explicit WhirlwindGenerator(const Vector3D& pos, float rad, float rInfl, float vs = 50.f, float k = 1.f);
	Vector3D getWindVelocity(const ParticleWithForces* p) const override;
private:
	Vector3D _pos;
	float _radiusInfluence;
	float _k; //constante proporcial a la fuerza del torbellino
	float _verticalStop;//unidades de distancia a las que dejara de aplicarse velocidad vertical
};