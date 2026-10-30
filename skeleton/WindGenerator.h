#pragma once
#include "ForceGenerator.h"
#include "SimulationConfig.h"

//SUPONEMOS QUE LOS OBJETOS QUE AFECTAN SON SIEMPRE UNIFORMES
class WindGenerator :public ForceGenerator {
public:
	//Predeterminado que use los params de referencia del aire
	explicit class WindGenerator(const Vector3D& vel, float radius, float densidad = 1.225f, float viscosidad = 1.81e-5f
		, float coefArraste = 0.47f);
	void updateForce(ParticleWithForces* p, float dt) override;
private:
	Vector3D _vel;
	float _k1, _k2;
	float _r;
	float _densidadAire;
	float _viscosidadAire;
	float _coefArraste;
};

/*
	float _densidadAire = 1.225f; //ref. normal a 15 grados en el nivel del mar
	float _viscosidadAire = 1.81e-5f;
	float _coefArraste = 0.47f; //Es la del objeto, en nuestro caso es aprox la de una esfera lisa
*/