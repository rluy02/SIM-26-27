#pragma once

#include <vector>
#include <algorithm>
#include "Scene.h"
#include "../RenderUtils.hpp"
#include "../Vector3D.h"
#include "../ParticleGenerator.h"
#include "../ParticleSystemWithForces.h"
#include "../GravityGenerator.h"

using TransformKey = std::string;

class P4S_Scene : public Scene {
public:
	explicit P4S_Scene(std::string name) : Scene(std::move(name)) {};

	void init() override {
		//Emisores-- Prueba de diferentes masas
		EmitterConfig cfg1 = sample();
		cfg1.mass = 1.f; //es la default
		cfg1.gravity = Vector3D(0.f, -9.81f, 0.f);
		cfg1.pos = Vector3D(-5.f, 0.f, 0.f);

		EmitterConfig cfg2 = sample();
		cfg2.mass = 100.f;
		cfg2.gravity = Vector3D(0.f, -9.81f, 0.f);
		cfg2.pos = Vector3D(5.f, 0.f, 0.f);
		cfg2.color = Vector4(0.5f, 0.5f, 0.5f, 1.f);

		//Generadores
		_gravity = new GravityGenerator();
		_megaGravity = new GravityGenerator(Vector3D(0.f, -30.f, 0.f));
		_forceGenerators.push_back(_gravity);
		_forceGenerators.push_back(_megaGravity);

		///Sistemas
		_systems.emplace_back(new ParticleSystemWithForces(cfg1));
		_systems.emplace_back(new ParticleSystemWithForces(cfg2));

		//registrar fuerzas
		_systems[0]->addForceGenerator(_gravity);
		_systems[0]->addForceGenerator(_gravity); //2 veces la gravedad

		_systems[1]->addForceGenerator(_megaGravity);

	}

	void update(double dt) override { for (ParticleSystemWithForces* s : _systems) s->update(dt); }

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

	}

	void cleanup() override {
		for (ParticleSystemWithForces* s : _systems) {
			delete s;
			s = nullptr;
		}
		for (ForceGenerator* s : _forceGenerators) {
			delete s;
			s = nullptr;
		}
		_systems.clear();
		_forceGenerators.clear();
	}
private:
	EmitterConfig sample() {
		EmitterConfig sample;
		sample.pos = Vector3D(0.f, 0.f, 0.f);
		sample.vel = Vector3D(0.f, 10.f, 0.f);
		sample.velSigma = Vector3D(0, 0, 0); //Para probar que caeran al mismo punto todas
		sample.lifeMin = 3.5f; sample.lifeMax = 8.5f;
		sample.meanPerFrame = 3.f; sample.varPerFrame = 2.f;
		sample.size = 1.5f;
		sample.color = Vector4(0.f, 1.0f, 1.f, 1.f);
		return sample;
	};
private:
	std::vector<ParticleSystemWithForces*> _systems;
	std::vector< ForceGenerator*> _forceGenerators; //para no tener que eliminar manualmente
	GravityGenerator* _gravity, * _megaGravity;
};
