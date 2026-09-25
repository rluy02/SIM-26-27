#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>
#include <unordered_map>

using TransformKey = std::string;

class P0S_Scene : public Scene {
public:
	explicit P0S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		physx::PxShape* shape = CreateShape(physx::PxSphereGeometry(1.0f)); //referencia compartida
		////RetoA(shape);
		//RetoB(shape);
		RetoC(shape);
		shape->release();
	}

	void update(double dt) override {
		// Lógica/Integración del alumno (por ejemplo, movimiento simple)
		//m_transform.p.y -= static_cast<float>(9.8 * dt);
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		if (key == 'r' || key == 'R') {
			//m_transform.p = physx::PxVec3(0.0f, 10.0f, 0.0f); // Reset
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
		m_transforms.clear();
	}

private:
	physx::PxTransform m_transform;
	physx::PxTransform m_transformE;
	std::unordered_map<TransformKey, physx::PxTransform> m_transforms;
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

		m_transforms.insert({ "m_transformU",physx::PxTransform(u) });
		m_transforms.insert({ "m_transformV",physx::PxTransform(v) });
		m_transforms.insert({ "m_transformW",physx::PxTransform(w) });

		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformU"), Vector4(1.0f, 0.0f, 0.0f, 1.0f)));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformV"), Vector4(0.0f, 1.0f, 0.0f, 1.0f)));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformW"), Vector4(0.0f, 0.0f, 1.0f, 1.0f)));
	}

	void RetoB(physx::PxShape* shape) {
		m_transformE = physx::PxTransform(Vector3D()); //Posi origen (0,0,0) enemigo
		Vector3D eDir(0.0f, 0.0f, 1.0f); //cono de vision del enemigo
		Vector3D P_1(2.0f, 0.0f, 3.0f), P_2(-4.0f, 0.0f, 1.0f), P_3(0.0f, 0.0f, -5.0f), P_4(3.0f, 0.0f, 0.0f);

		m_transforms.insert({ "m_transformP1",physx::PxTransform(P_1) });
		m_transforms.insert({ "m_transformP2",physx::PxTransform(P_2) });
		m_transforms.insert({ "m_transformP3",physx::PxTransform(P_3) });
		m_transforms.insert({ "m_transformP4",physx::PxTransform(P_4) });

		m_renderItems.emplace_back(new RenderItem(shape, &m_transformE, Vector4(0.f, 0.f, 0.f, 1.f)));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformP1"), checkAng(eDir.dot(P_1))));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformP2"), checkAng(eDir.dot(P_2))));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformP3"), checkAng(eDir.dot(P_3))));
		m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transformP4"), checkAng(eDir.dot(P_4))));
	}

	Vector4 checkAng(float ang) {
		if (ang > 0) { //delante
			return Vector4(0.0f, 1.0f, 0.0f, 1.0f); //verde
		}
		else if (ang < 0) { //detras
			return Vector4(1.0f, 0.0f, 0.0f, 1.0f); //rojo
		}
		else {
			return Vector4(1.0f, 1.0f, 0.0f, 1.0f); //amarillo
		}
	}

	void RetoC(physx::PxShape* shape) {
		Vector3D A(-8.0, 1.0, -8.0), B(8.0, 8.0, 8.0);

		float t = 0.f;
		for (int i = 0;i < 10;i++) {

			Vector3D P_t = (B - A) * t + A; //interpolacion (puesto asi porque el operador recibe Vector * escalar)
			t += 0.1f; //incr
			m_transforms.insert({ "m_transform" + i, physx::PxTransform(P_t) });
			m_renderItems.emplace_back(new RenderItem(shape, &m_transforms.at("m_transform" + i), Vector4(0.f, 0.f, 0.f, 1.f)));
		}

	}
};