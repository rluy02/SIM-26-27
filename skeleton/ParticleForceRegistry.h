#pragma once
#include <vector>
#include <algorithm>
#include "Generators/ForceGenerator.h"

class ParticleForceRegistry {
public:
	//------ Impl. Generica ------
	inline void add(ForceGenerator* generator) { _forceGenerators.push_back(generator); }
	void remove(ForceGenerator* generator) {
		auto it = std::remove(
			_forceGenerators.begin(),
			_forceGenerators.end(),
			generator
		);//busqueda

		_forceGenerators.erase(it, _forceGenerators.end());//eliminar el rango
	}

	void applyForces(ParticleWithForces* particle, float dt) {
		for (ForceGenerator* const fg : _forceGenerators) {
			fg->updateForce(particle, dt); //que cada uno aplique lo que corresponda, se puede ir acumulando si tiene varias
		}

		//solo si aplica (muelles)
		for (const Registration& r : _registrations)       // los asociados, solo a su particula
			if (r.particle == particle)
				r.generator->updateForce(particle, dt);
	}
	//Para consultar
	bool has(const ForceGenerator* generator) const {
		return std::find(_forceGenerators.begin(), _forceGenerators.end(), generator) != _forceGenerators.end();
	}

	size_t size() const { return _forceGenerators.size(); } //solo cuenta los generales
	//------------------------------------------------------------------
	//------ Generadores asociados a UNA sola particula (muelles) ------
	inline void add(ForceGenerator* generator, ParticleWithForces* particle) {
		_registrations.push_back({ generator, particle });
	}
	void remove(ForceGenerator* generator, ParticleWithForces* particle) {
		auto it = std::remove_if(_registrations.begin(), _registrations.end(),
			[generator, particle](const Registration& r) { return r.generator == generator && r.particle == particle; });

		_registrations.erase(it, _registrations.end());
	}
	bool has(const ForceGenerator* generator, const ParticleWithForces* particle) const {

		for (const Registration& r : _registrations) {
			if (r.generator == generator && r.particle == particle)
				return true;
		}

		return false;
	}
	inline void clear() { _forceGenerators.clear(); _registrations.clear(); } // sin destruir, solo limpia

private:
	std::vector<ForceGenerator*> _forceGenerators;
	//Impl para poder asociar a las particulas (sin ser sistema) un generador
	struct Registration { ForceGenerator* generator; ParticleWithForces* particle; };
	std::vector<Registration> _registrations;
};