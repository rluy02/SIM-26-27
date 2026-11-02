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

class P4S_Scene : public Scene {
public:
	explicit P4S_Scene(std::string name) : Scene(std::move(name)) {};

	void init() override {
		//Emisores-- Prueba de diferentes masas
		//Nota: a igual masa mismo tamaño se deja llevar mas la de mayor tamaño porque ofrece mas area de contacto con el viento
		//ejemplo: globo vs pelota
		EmitterConfig ball = sample();
		ball.mass = 1.f; //es la default
		ball.pos = Vector3D(-5.f, 0.f, 0.f);
		ball.size = 0.1f;

		EmitterConfig balloon = sample();
		balloon.mass = 1.f;
		balloon.pos = Vector3D(5.f, 0.f, 0.f);
		balloon.color = Vector4(0.5f, 0.5f, 0.5f, 1.f);
		balloon.size = 0.5f;

		EmitterConfig rocks = sample();
		rocks.mass = 0.1f; //es la default
		rocks.pos = Vector3D(0.f, 0.f, 0.f);
		rocks.size = 0.25f;
		rocks.color = Vector4(0.1, 0.1, 0.5, 1.f);

		//----Ejemplo Gravedad----
		//pruebaGravedad(ball, balloon);
		//----Ejemplo Viento----
		//pruebaViento(ball, balloon); //la cfg1 le afectara mas el viento por su peso
		//----Ejemplo Torbellino----
		//pruebaTorbellino(sampleWhirl());
		//----Ejemplo Explosion----
		pruebaExplosion(rocks);

	}

	void update(double dt) override {
		for (ExplosionGenerator* explosion : _explosions) //primero actualizar la explosions
			explosion->update(dt);

		for (ParticleSystemWithForces* s : _systems)
			s->update(dt);

	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		//activa todas las explosiones a la vez
		if (key == 'e' || key == 'E')
			for (ExplosionGenerator* explosion : _explosions)
				explosion->activate();
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
		for (ExplosionGenerator* s : _explosions) {
			delete s;
			s = nullptr;
		}
		_systems.clear();
		_forceGenerators.clear();
		_explosions.clear();
	}
private:
	ParticleSystemWithForces* createSystem(const EmitterConfig& cfg) {
		ParticleSystemWithForces* system = new ParticleSystemWithForces(cfg);
		_systems.push_back(system);
		return system;
	}
	template <typename T>
	T* addGenerator(T* generator) {
		_forceGenerators.push_back(generator);
		return generator;
	}

	void pruebaGravedad(const EmitterConfig& cfg1, const EmitterConfig& cfg2) {
		auto* gravity = addGenerator(new GravityGenerator());
		auto* megaGravity = addGenerator(new GravityGenerator(Vector3D(0.f, -30.f, 0.f)));

		auto* system1 = createSystem(cfg1);
		auto* system2 = createSystem(cfg2);

		system1->addForceGenerator(gravity);
		system2->addForceGenerator(megaGravity);
	};
	void pruebaViento(const EmitterConfig& cfg1, const EmitterConfig& cfg2) {
		//Vientos
		auto* wind = addGenerator(new WindGenerator(Vector3D(15.f, 0.f, 0.f), cfg1.size));
		auto* wind2 = addGenerator(new WindGenerator(Vector3D(15.f, 0.f, 0.f), cfg1.size));
		///Sistemas
		auto* system1 = createSystem(cfg1);
		auto* system2 = createSystem(cfg2);
		//registrar fuerzas
		system1->addForceGenerator(wind);
		system2->addForceGenerator(wind2);
	};

	void pruebaTorbellino(const EmitterConfig& cfg1) {
		//se debilita la velocidad vertical mientras sube, a partir de 30.f ira decayendo
		//el radio de influencia del torbellino es 20*20.f
		auto* whirl = addGenerator(new WhirlwindGenerator(Vector3D(0.f, 0.f, 0.f), cfg1.size, 20.f, 30.f, 1.f)); //default en el centro del mundo

		auto* system = createSystem(cfg1);

		system->addForceGenerator(whirl);
	};
	void pruebaExplosion(const EmitterConfig& cfg1) {
		//explosion mas duradera aprox tau=3.f; mas rapida =0.3f. K es la el factor de fuerza de la explosion
		auto* explosion = new ExplosionGenerator(Vector3D(0.f, 0.f, 0.f), 50.f, 40.f, 1.0f);
		auto* gravity = addGenerator(new GravityGenerator());

		auto* system = createSystem(cfg1);

		system->addForceGenerator(explosion);
		_explosions.push_back(explosion);
	};
	//no hace falta definir la gravedad. No es algo propio a calcular en la particula con fuerzas
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
	EmitterConfig sampleWhirl() {
		EmitterConfig tornado;
		tornado.pos = Vector3D(0.f, 0.f, 0.f);
		tornado.posSigma = Vector3D(5.5f, 0.f, 5.5f);
		tornado.vel = Vector3D(0.f, 2.f, 0.f);
		tornado.velSigma = Vector3D(0.5f, 1.f, 0.5f);
		tornado.meanPerFrame = 15.f;
		tornado.varPerFrame = 3.f;
		tornado.lifeMin = 10.5f; tornado.lifeMax = 20.5f;
		tornado.size = 0.15f;
		tornado.mass = 0.2f;
		return tornado;
	};
private:
	std::vector<ParticleSystemWithForces*> _systems;
	std::vector< ForceGenerator*> _forceGenerators; //para no tener que eliminar manualmente
	std::vector<ExplosionGenerator*> _explosions; //para simular varias explosiones
};
