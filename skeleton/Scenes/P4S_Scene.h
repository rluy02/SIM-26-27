#pragma once

#include <vector>
#include <algorithm>
#include "Scene.h"
#include "../RenderUtils.hpp"
#include "../Vector3D.h"
#include "../ParticleGenerator.h"
#include "../ParticleSystemWithForces.h"
#include "../GravityGenerator.h"
#include "../WindGenerator.h"

using TransformKey = std::string;

class P4S_Scene : public Scene {
public:
	explicit P4S_Scene(std::string name) : Scene(std::move(name)) {};

	void init() override {
		//Emisores-- Prueba de diferentes masas
		//Nota: a igual masa mismo tamaño se deja llevar mas la de mayor tamaño porque ofrece mas area de contacto con el viento
		//ejemplo: globo vs pelota
		EmitterConfig ball = sample();
		ball.mass = 1.f; //es la default
		ball.gravity = Vector3D(0.f, -9.81f, 0.f);
		ball.pos = Vector3D(-5.f, 0.f, 0.f);
		ball.size = 0.1f;

		EmitterConfig balloon = sample();
		balloon.mass = 1.f;
		balloon.gravity = Vector3D(0.f, -9.81f, 0.f);
		balloon.pos = Vector3D(5.f, 0.f, 0.f);
		balloon.color = Vector4(0.5f, 0.5f, 0.5f, 1.f);
		balloon.size = 0.5f;


		//pruebaGravedad(cfg1, cfg2);
		pruebaViento(ball, balloon); //la cfg1 le afectara mas el viento por su peso

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
	void pruebaGravedad(const EmitterConfig& cfg1, const EmitterConfig& cfg2) {
		_gravity = new GravityGenerator();
		_megaGravity = new GravityGenerator(Vector3D(0.f, -30.f, 0.f));
		_forceGenerators.push_back(_gravity);
		_forceGenerators.push_back(_megaGravity);

		///Sistemas
		_systems.emplace_back(new ParticleSystemWithForces(cfg1));
		_systems.emplace_back(new ParticleSystemWithForces(cfg2));
		//registrar fuerzas
		_systems[0]->addForceGenerator(_gravity);
		_systems[1]->addForceGenerator(_megaGravity);
	};
	void pruebaViento(const EmitterConfig& cfg1, const EmitterConfig& cfg2) {
		//Viento
		_wind = new WindGenerator(Vector3D(15.f, 0.f, 0.f), cfg1.size/2.f);
		_wind2 = new WindGenerator(Vector3D(15.f, 0.f, 0.f), cfg2.size/2.f);
		_forceGenerators.push_back(_wind);
		_forceGenerators.push_back(_wind2);
		///Sistemas
		_systems.emplace_back(new ParticleSystemWithForces(cfg1));
		_systems.emplace_back(new ParticleSystemWithForces(cfg2));
		//registrar fuerzas
		_systems[0]->addForceGenerator(_wind);
		_systems[1]->addForceGenerator(_wind2);
	};
	EmitterConfig sample() {
		EmitterConfig sample;
		sample.pos = Vector3D(0.f, 0.f, 0.f);
		sample.posSigma = Vector3D(2.f, 0.f, 2.f);
		sample.vel = Vector3D(0.f, 10.f, 0.f);
		sample.velSigma = Vector3D(0, 3.f, 0); //Para probar que caeran al mismo punto todas
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
	WindGenerator* _wind, * _wind2;
};
