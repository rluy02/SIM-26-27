#pragma once

#include <vector>
#include <algorithm>
#include "Scene.h"
#include "../RenderUtils.hpp"
#include "../Vector3D.h"
#include "../ParticleGenerator.h"
#include "../ParticleSystemWithForces.h"

using TransformKey = std::string;

class P4S_Scene : public Scene {
public:
	explicit P4S_Scene(std::string name) : Scene(std::move(name)) {};

	void init() override {
		_sys = new ParticleSystemWithForces(simple());
		//reg fuerza
	}

	void update(double dt) override { _sys->update(dt); }

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

	}

	void cleanup() override {
		delete _sys;
	}
private:
	EmitterConfig simple() {
		EmitterConfig simple;
		simple.pos = Vector3D(0.f, 0.f, 0.f);
		simple.vel = Vector3D(0.f, 10.f, 0.f);
		simple.velSigma = Vector3D(10.f, 5.f, 10.f);
		simple.lifeMin = 3.5f; simple.lifeMax = 8.5f;
		simple.meanPerFrame = 3.f; simple.varPerFrame = 2.f;
		simple.size = 1.5f;
		simple.color = Vector4(0.f, 1.0f, 1.f, 1.f);
		return simple;
	};
private:
	ParticleSystemWithForces* _sys;
};
