#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>
#include <algorithm>
#include "ParticleSystem.h"
#include "ParticleGenerator.h"

using TransformKey = std::string;

class P3S_Scene : public Scene {
public:
	explicit P3S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		_systems.emplace_back(new ParticleSystem(manguera()));
		_systems.emplace_back(new ParticleSystem(niebla()));
		_systems.emplace_back(new ParticleSystem(chispas()));
	}

	void update(double dt) override { for (ParticleSystem* s : _systems) s->update(dt); }

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

	}

	void cleanup() override {
		for (ParticleSystem* s : _systems) {
			delete s;
			s = nullptr;
		}
		_systems.clear();
	}
private:
	//sale de un punto, velocidad con dispersion
	EmitterConfig manguera() {
		EmitterConfig manguera;
		manguera.pos = Vector3D(-15.f, 0.f, 0.f);
		manguera.vel = Vector3D(0.f, 15.f, 0.f);
		manguera.velSigma = Vector3D(1.f, 1.f, 1.f);
		manguera.lifeMin = 1.5f; manguera.lifeMax = 2.5f;
		manguera.meanPerFrame = 3.f; manguera.varPerFrame = 1.f;
		manguera.size = 0.3f;
		manguera.color = Vector4(0.f, 0.5f, 1.f, 1.f);
		return manguera;
	};

	//posicion muy variable, velocidad pequena, particulas pequenas
	EmitterConfig niebla() {
		EmitterConfig niebla;
		niebla.pos = Vector3D(0.f, 3.f, 0.f);
		niebla.posSigma = Vector3D(5.f, 1.f, 5.f);
		niebla.vel = Vector3D(0.f, 0.5f, 0.f);
		niebla.velSigma = Vector3D(0.5f, 0.2f, 0.5f);
		//niebla.gravity = Vector3D(0.f, -2.f, 0.f); //probando con menos gravedad que la normal
		niebla.lifeMin = 3.f; niebla.lifeMax = 6.f;
		niebla.meanPerFrame = 30.5f; niebla.varPerFrame = 10.5f;
		niebla.damping = 0.02f;
		niebla.size = 0.2f;
		niebla.color = Vector4(0.8f, 0.8f, 0.8f, 1.f);
		return niebla;
	};

	//vida corta, salida hacia arriba con dispersion horizontal
	EmitterConfig chispas() {
		EmitterConfig chispas;
		chispas.pos = Vector3D(15.f, 0.f, 0.f);
		chispas.vel = Vector3D(0.f, 10.f, 0.f);
		chispas.velSigma = Vector3D(3.f, 2.f, 3.f);
		chispas.lifeMin = 0.5f; chispas.lifeMax = 1.5f;
		chispas.meanPerFrame = 2.f; chispas.varPerFrame = 1.f;
		chispas.size = 0.15f;
		chispas.color = Vector4(1.f, 0.5f, 0.f, 1.f);
		return chispas;
	};
private:
	std::vector<ParticleSystem*> _systems;

};

//Pos con sigma pequeno: fuente concentrada. Con grande: dispersa
//VelSigma cambia la dispersion respecto al movimiento original
//damping y vida es la logica tal cual
//meanPerFrame y varPerFrame es la media de particulas y su variacion respecto a cuantas pueden generarse en "un" frame