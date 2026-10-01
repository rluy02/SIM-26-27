#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>
#include <unordered_map>
#include "Particle.h"

using TransformKey = std::string;

class P1S_Scene : public Scene {
public:
	explicit P1S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		_particle = new Particle(INI_POS, INI_VEL, INI_ACC, INI_DUMP);
	}

	void update(double dt) override {
		_particle->integrate(dt);
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		if (key == 'r' || key == 'R') {
			//m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
		}
	}

	void cleanup() override {
		delete _particle;_particle = nullptr;
		if (!m_renderItems.empty()) {
			for (RenderItem* ri : m_renderItems) {
				ri->release(); // Deregistra y destruye el item
				ri = nullptr;
			}
		}
		m_renderItems.clear();
		m_transforms.clear();
	}

private:
	Particle* _particle;
	const Vector3D INI_POS = Vector3D();
	const Vector3D INI_VEL = Vector3D(30.f, 0.f, 0.f);
	const Vector3D INI_ACC = Vector3D(-2.f, 0.f, 0.f);
	static constexpr double INI_DUMP = 0.80;
	std::unordered_map<TransformKey, physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;

private:
};