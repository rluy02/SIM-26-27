#pragma once
#include "Particle.h"
class Projectile : public Particle
{
public:
	explicit Projectile(const Vector3D& pos, const Vector3D& vel, const Vector3D& acc, bool shootFromCamera, const double mass = 0.0075, const double damping = 1.00);

public:
	inline void modifyMass(double valAccAdded) { _mass += valAccAdded; };
	void modifyVelocity(double valAccAdded);
	inline bool WasShootFromCamera() const { return _shootFromCamera; };
private:
	bool _shootFromCamera = false;
	//Masa real promedio: 0.0075kg = 7.5g
	//Velocidad real promedio: 340m/s
};


//El proyectil simulado ha de mantener un compartamiento realista respecto a una real.
//Como trabjaremos con distinta velocidad o masas para que esta sea p. ej, mas lenta. 
//Habra que igualar la energia (esto logra producir mismo efecto en una colision) "Querremos saber como actua la masa simulada"
// Es==Er => 0.5*Ms*Vs^2=0.5*Mr*Vr^2 ...... Ms=Mr*Vr^2/Vs^2
//Aplica igual para simular la parabola cuando tenga que empezar a caer el proyectil. "Querremos saber como actua la gravedad simulada"
// Dr==Ds => raiz(2*Hr/Gr)*Vr=raiz(2*Hs/Gs)*Vs ...... Gs=(Vs^2/Vr^2)*Gr

//Para que se dispare a partir de la camara:
//GetCamera->getDir
//GetCamera->getEye