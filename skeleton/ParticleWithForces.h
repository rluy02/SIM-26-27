#pragma once
#include "Vector3D.h"
#include "RenderUtils.hpp"

//Impl. en una clase aparte para mantener versionado lo hecho hasta ahora
class ParticleWithForces
{
public:
	explicit ParticleWithForces(const Vector3D& pos, const Vector3D& vel, const float mass, const float damping = 1.00f,
		const float pSize = 10.f, const Vector4& color = Vector4(0.f, 1.f, 1.f, 1.f));
	virtual ~ParticleWithForces();
	//Diferente metodo de integracion según para que.
	virtual void integrateVerlet(double t);
	virtual void integrateSEE(double t); //Más conveniente para el sistema
	void showParamsDebug();
	void clearForces();
	void addForce(const Vector3D& force);
	inline float getMass() const { return _mass; };
	inline const Vector3D& getPosition() const {return _pos.p;}
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


//Nota aclaratoria:
/*
En la particula sin fuerzas trataba la aceleracion como fuerzas, por eso elimino la variable de aqui para mejor entendimiento.
A efectos practicos cuando se hable de aceleracion sera una variable local y siempre se genera a partir de las fuerzas actualizadas
Quizas vendria bien guardarla como miembro si en un futuro necesito saber la acc de un frame previo o algo asi
*/