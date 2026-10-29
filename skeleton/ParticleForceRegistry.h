#pragma once
#include <vector>
#include "ForceGenerator.h"
class ParticleForceRegistry {
public:
	inline void add(ForceGenerator* generator) { _forceGenerators.push_back(generator); }
	void remove(ForceGenerator* generator) {
		auto it = std::remove(
			_forceGenerators.begin(),
			_forceGenerators.end(),
			generator
		);//busqueda

		_forceGenerators.erase(it, _forceGenerators.end());//eliminar el rango
	}
	//el clear podria ser mas como un reset de todos los generadores si hiciese falta
	inline void clear() { _forceGenerators.clear(); };//solo limpia el vec, los generadores se eliminan en la escena

	void applyForces(ParticleWithForces* particle, float dt) {
		for (ForceGenerator* const fg : _forceGenerators) {
			fg->updateForce(particle, dt); //que cada uno aplique lo que corresponda, se puede ir acumulando si tiene varias
		}
	}

private:
	std::vector<ForceGenerator*> _forceGenerators;
};