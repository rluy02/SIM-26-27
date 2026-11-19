#pragma once

#include <vector>
#include <algorithm>
#include "Scene.h"
#include "../RenderUtils.hpp"
#include "../Vector3D.h"
#include "../ParticleSystemWithForces.h"
#include "Generators/ParticleGenerator.h"
#include "Generators/GravityGenerator.h"
#include "Generators/WindGenerator.h"
#include "Generators/WhirlwindGenerator.h"
#include "Generators/ExplosionGenerator.h"
#include "Generators/SpringForceGenerator.h"
#include "Generators/AnchoredSpringFG.h"
#include "imgui.h"

class P6S_Scene : public Scene {
public:
	explicit P6S_Scene(std::string name) : Scene(std::move(name)) {};

	void init() override {

		ejemplo();
		//--------
		actividad1();
		//--------

		//General a todo lo creado
		_push = addGenerator(new ExplosionGenerator(Vector3D(0.f, 5.f, 0.f), 300.f, 50.f, 0.3f)); // centro, K, radio, tau
		_registry.add(_push);
	}

	void update(double dt) override {
		_push->update(dt); // avanza el tiempo de la explosion

		for (ParticleWithForces* p : _particles) {   // fuerzas de todas con las posiciones del mismo instante
			p->clearForces();
			_registry.applyForces(p, static_cast<float>(dt));
		}
		for (ParticleWithForces* p : _particles) {   // después se integra
			if (_useExplicitEuler) p->integrateEE(dt); //acumula energia MUCHO mas rapido hasta salir de control
			else                   p->integrateSEE(dt);
		}
	}

	void drawDebugUI() override {
		if (_spring)ImGui::Text("k = %.1f", _spring->getK());
		ImGui::Text("Integrador: %s", _useExplicitEuler ? "Euler explicito" : "Euler semi-implicito");
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		switch (key) {
		case 'e': case 'E': _push->activate(); break;                                  // fuerza breve
		case '+': if (_spring) _spring->setK(_spring->getK() + _kStep); break;            // sube k
		case '-': if (_spring)   _spring->setK(max(0.f, _spring->getK() - _kStep)); break; // baja k
		case 'm': case 'M': _useExplicitEuler = !_useExplicitEuler; break;             // cambia integrador
		}
	}

	void cleanup() override {
		for (ForceGenerator* g : _forceGenerators) delete g; // el AnchoredSpringFG libera su ancla solo
		for (ParticleWithForces* p : _particles) delete p;
		_forceGenerators.clear();
		_particles.clear();
		_registry.clear();
	}
private:
	//A mayor k es mas rigido, oscila mas rapido
	void ejemplo() {
		// Pareja unida por un muelle (p2 pesa el doble, como en el demo del profe)
		_p1 = addParticle(new ParticleWithForces(Vector3D(-10.f, 30.f, 0.f), Vector3(0.f), 1.f, 1.f, 1.f, Vector4(1.f, 0.5f, 0.f, 1.f)));
		_p2 = addParticle(new ParticleWithForces(Vector3D(10.f, 30.f, 0.f), Vector3(0.f), 2.f, 1.f, 1.5f, Vector4(1.f, 0.f, 0.5f, 1.f)));

		_f1 = addGenerator(new SpringForceGenerator(20.f, 10.f, _p2)); // actúa sobre p1, el otro extremo es p2
		_f2 = addGenerator(new SpringForceGenerator(20.f, 10.f, _p1)); // actúa sobre p2, el otro extremo es p1

		_registry.add(_f1, _p1);
		_registry.add(_f2, _p2);
	}
	void actividad1() {
		//																								  damping
		_ball = addParticle(new ParticleWithForces(Vector3D(0.f, 10.f, 0.f), Vector3D(0.f, 0.f, 0.f), 1.f, 0.7f, 1.f,
			Vector4(0.f, 1.f, 1.f, 1.f)));

		_gravity = addGenerator(new GravityGenerator());
		_spring = addGenerator(new AnchoredSpringFG(20.f, 10.f, Vector3D(0.f, 25.f, 0.f)));

		_registry.add(_gravity, _ball);      // la gravedad solo afecta a la bola
		_registry.add(_spring, _ball);       // el muelle anclado SOLO a la bola
	}
	void actividad2() {

	}

	template <typename T>
	T* addGenerator(T* generator) {
		_forceGenerators.push_back(generator);
		return generator;
	}
	ParticleWithForces* addParticle(ParticleWithForces* p) { _particles.push_back(p); return p; }

private:

	ParticleWithForces* _ball = nullptr, * _p1 = nullptr, * _p2 = nullptr;
	ParticleForceRegistry _registry;
	std::vector<ForceGenerator*> _forceGenerators;
	std::vector<ParticleWithForces*> _particles;
	SpringForceGenerator* _f1 = nullptr, * _f2 = nullptr;
	GravityGenerator* _gravity = nullptr;
	AnchoredSpringFG* _spring = nullptr;
	ExplosionGenerator* _push = nullptr;

	bool _useExplicitEuler = false;
	float _kStep = 50.f; // lo q cambia k por pulsacion
};
