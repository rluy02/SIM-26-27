#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"

//Impl. en una clase aparte para mantener versionado lo hecho hasta ahora
class ParticleWithForces
{
public:
	explicit ParticleWithForces(const Vector3D& pos, const Vector3D& vel, const Vector3D& f, const float mass, const float damping = 1.00f,
		const float pSize = 10.f, const Vector4& color = Vector4(0.f, 1.f, 1.f, 1.f));
	virtual ~ParticleWithForces();
	//Diferente metodo de integracion según para que.
	virtual void integrateVerlet(double t);
	virtual void integrateSEE(double t); //Más conveniente para el sistema
	void showParamsDebug();
private:
	void semiExplicitEuler(float t);
	void verlet(float t);
protected:
	Vector3D _vel;
	Vector3D _f;
	float _mass;
	float _invMass;//w
	float _damping; //dumping
	float _dampingFactor;
	Vector3D _prevPos; //para verlet
	physx::PxTransform _pos;
	RenderItem* _renderItem;
};


