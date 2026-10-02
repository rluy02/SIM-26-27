#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>
#include <unordered_map>
#include "Projectile.h"

using TransformKey = std::string;

class P2S_Scene : public Scene {
public:
	explicit P2S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		_proj = new Projectile(INI_POS, INI_VEL, INI_ACC, false, INI_MASS, INI_DUMP);
	}

	void update(double dt) override {
		_proj->integrate(dt);
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		const double ValFactor = 0.1;
		//Cambiar velocidad real del proyectil (cambia internamente segun si instancio desde camara o no)
		if (key == 'j' || key == 'J') {
			_proj->modifyVelocity(-ValFactor);
		}
		if (key == 'k' || key == 'K') {
			_proj->modifyVelocity(ValFactor);
		}
		//Cambiar masa real del proyectil
		if (key == 'g' || key == 'G') {
			_proj->modifyMass(-ValFactor);
		}
		if (key == 'h' || key == 'H') {
			_proj->modifyMass(ValFactor);
		}

		if (key == 'i' || key == 'I') {
			_proj->showParamsDebug();
		}
	}

	void cleanup() override {
		delete _proj;_proj = nullptr;
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
	Projectile* _proj;
	const Vector3D INI_POS = Vector3D();
	const Vector3D INI_VEL = Vector3D(30.f, 0.f, 0.f);
	const Vector3D INI_ACC = Vector3D(-2.f, 0.f, 0.f);
	static constexpr double INI_DUMP = 0.80;
	static constexpr double INI_MASS = 0.0075;
	std::unordered_map<TransformKey, physx::PxTransform> m_transforms;
	std::vector<RenderItem*> m_renderItems;
};