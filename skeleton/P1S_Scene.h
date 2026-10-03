#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "Particle.h"

using TransformKey = std::string;

class P1S_Scene : public Scene {
public:
	explicit P1S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		_particle = new Particle(INI_POS, INI_VEL, INI_ACC, INI_MASS, INI_DUMP);
	}

	void update(double dt) override {
		_particle->integrate(dt);
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

		if (key == 'i' || key == 'I') {
			_particle->showParamsDebug();
		}
	}

	void cleanup() override {
		delete _particle;_particle = nullptr;
	}

private:
	Particle* _particle;
	const Vector3D INI_POS = Vector3D(0, 0, 0);
	const Vector3D INI_VEL = Vector3D(30.f, 0.f, 0.f);
	const Vector3D INI_ACC = Vector3D(-2.f, 0.f, 0.f);
	static constexpr double INI_MASS = 0.0005;
	static constexpr double INI_DUMP = 0.80;

private:
};