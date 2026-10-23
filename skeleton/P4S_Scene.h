#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>
#include <algorithm>
#include "ParticleGenerator.h"

using TransformKey = std::string;

class P4S_Scene : public Scene {
public:
	explicit P4S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
	}

	void update(double dt) override { }

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

	}

	void cleanup() override {
	}
private:

};
