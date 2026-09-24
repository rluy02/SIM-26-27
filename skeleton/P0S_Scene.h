#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>

class P0S_Scene : public Scene {
public:
	explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(2.0f)); //referencia compartida
		RetoA(shape);
		shape->release();
	}

	void update(double dt) override {
		// Lógica/Integración del alumno (por ejemplo, movimiento simple)
		//m_transform.p.y -= static_cast<float>(9.8 * dt);
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		if (key == 'r' || key == 'R') {
			m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
		}

	}

	void cleanup() override {
		if (!m_renderItems.empty()) {
			for (RenderItem* ri : m_renderItems) {
				ri->release(); // Deregistra y destruye el item
				ri = nullptr;
			}
		}
		m_renderItems.clear();
	}

private:
	physx::PxTransform m_transform;
	physx::PxTransform m_transformU;
	physx::PxTransform m_transformV;
	physx::PxTransform m_transformW;
	std::vector<RenderItem*> m_renderItems;

private:
	void RetoA(physx::PxShape* shape)
	{
		Vector3D u(3.0f, 1.0f, 0.0f); //Eje X Rojo
		Vector3D v(0.0f, 4.0f, 0.0f); //Eje Y Verde

		Vector3D w = u.cross(v); //Eje Z (Normal de X-Y) Azul

		//normalizar y escalar
		float scale_m = 5.f;
		u = u.normalize() * scale_m;
		v = v.normalize() * scale_m;
		w = w.normalize() * scale_m;

		m_transformU = physx::PxTransform(u); //hace la conversion implicita
		m_transformV = physx::PxTransform(v);
		m_transformW = physx::PxTransform(w);

		m_renderItems.emplace_back(new RenderItem(shape, &m_transformU, Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transformV, Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transformW, Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
	}
};