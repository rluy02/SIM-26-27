#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"
class Particle
{
public:
	explicit Particle(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, const double mass, const double damping = 1.00,
		const float pSize = 10.f, const Vector4& color = Vector4(0.f, 1.f, 1.f, 1.f));
	virtual ~Particle();
	virtual void integrate(double t);
	void showParamsDebug();
private:
	void explicitEuler(double t);
	//Opcional
	void semiExplicitEuler(double t);
	void verlet(double t);
protected:
	Vector3D _vel;
	Vector3D _acc;
	double _mass;
	double _damping; //dumping
	Vector3D _prevPos; //para verlet
	physx::PxTransform _pos;
	RenderItem* _renderItem;
};


