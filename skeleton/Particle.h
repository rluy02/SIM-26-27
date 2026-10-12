#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
class Particle
{
public:
	explicit Particle(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, const float mass, const float damping = 1.00f,
		const float pSize = 10.f, const Vector4& color = Vector4(0.f, 1.f, 1.f, 1.f));
	virtual ~Particle();
	//Diferente metodo de integracion según para que.
	virtual void integrateVerlet(double t);
	virtual void integrateSEE(double t); //Más conveniente para el sistema
	void showParamsDebug();
private:
	void explicitEuler(float t);
	//Opcional
	void semiExplicitEuler(float t);
	void verlet(float t);
protected:
	Vector3D _vel;
	Vector3D _acc;
	float _mass;
	float _damping; //dumping
	float _dampingFactor;
	Vector3D _prevPos; //para verlet
	physx::PxTransform _pos;
	RenderItem* _renderItem;
};


