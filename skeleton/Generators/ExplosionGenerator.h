#pragma once
#include "ForceGenerator.h"

class ExplosionGenerator : public ForceGenerator
{
public:
	explicit ExplosionGenerator(const Vector3D& pos, float k, float _radiusInfluence, float constTiempo);
	void updateForce(ParticleWithForces* p, float dt) override;

	void update(double dt); //una vez por frame en la escena
	void activate(); //evento teclado que se llama en la escena
private:
	Vector3D _pos;
	float _k;
	float _radiusInfluence;
	float _constTiempo; //tau
	float _elapsedTime;
	bool _active;
};