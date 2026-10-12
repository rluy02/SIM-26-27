#pragma once

#include "Scene.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <vector>
#include <algorithm>
#include "Projectile.h"

using TransformKey = std::string;

struct ProjectileType {
	std::string name;
	float rMass;    // kg   (real)
	float rVel;     // m/s  (real)
	float rGrav;    // m/s^2 (real; positiva = flota)
	float sVel;     // m/s  (la elegimos nosotros)
	float damping;
	float size;
	Vector4 color;
	float sMass = 0.0f;
	float sGrav = 0.0f;

	ProjectileType(std::string n, float rm, float rv, float rg, float sv, float d, float s, const Vector4& c)
		: name(std::move(n)), rMass(rm), rVel(rv), rGrav(rg), sVel(sv), damping(d), size(s), color(c) {
		updateSimulated(); //CALCULO DE ESCALADO AL CREARSE
	}

	void updateSimulated() {
		// Misma energia cinetica: Ms = Mr * Vr^2 / Vs^2
		sMass = rMass * (rVel * rVel) / (sVel * sVel);
		// Misma parabola: Gs = Gr * Vs^2 / Vr^2 (el signo de Gr se conserva)
		sGrav = rGrav * (sVel * sVel) / (rVel * rVel);
	}

	void showDebug() const {
		std::cout << "[" << name << "] Real: m=" << rMass << " kg, v=" << rVel
			<< " m/s, g=" << rGrav << " m/s^2 || : m=" << sMass
			<< " kg, v=" << sVel << " m/s, g=" << sGrav << " m/s^2" << std::endl;
	}
};

class P2S_Scene : public Scene {
public:
	explicit P2S_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {
		_projsTypes.clear();
		const Vector4 r(1.f, 0.f, 0.f, 1.f);
		const Vector4 g(0.f, 1.f, 0.f, 1.f);
		const Vector4 b(0.f, 0.f, 1.f, 1.f);
		//nombre, rMass, rVel, rGrav, sVel, damping, size, color
		_projsTypes.emplace_back("Bala", 0.0075f, 340.0f, simulation::worldGravityY, 30.0f, 1.0f, 2.f, r);
		_projsTypes.emplace_back("Bola de canon", 5.0f, 300.0f, simulation::worldGravityY, 40.0f, 1.0f, 6.f, g);
		_projsTypes.emplace_back("Globo de helio", 0.003f, 5.0f, 2.0f, 5.0f, 0.9f, 12.f, b);
		_sel = 0;
	}

	void update(double dt) override {
		const size_t n = _projs.size();
		for (size_t i = 0u; i < n; i++) //mas seguro que el for each
		{
			_projs.at(i)->integrateSEE(dt);
		}
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

		ProjectileType& t = _projsTypes[_sel]; //Por ref para poder cambiar sVel o rMass

		//cambiar tipo de proyectil
		if (key == 'c' || key == 'C') {
			_sel = (_sel + 1) % _projsTypes.size();
			ProjectileType pt = _projsTypes[_sel]; //copia para mostrar el actual
			pt.showDebug();
		}

		if (key == ' ') { shoot(t); return; } // dispara el tipo seleccionado

		//Cambiar velocidad simulada del proyectil (SE EFECTUA PARA EL SIGUIENTE A GENERARSE)
		if (key == 'j' || key == 'J') {
			t.sVel = physx::PxClamp(t.sVel * VAL_FACTOR, VEL_MIN, VEL_MAX);
			t.updateSimulated();
		}
		if (key == 'k' || key == 'K') {
			t.sVel = physx::PxClamp(t.sVel / VAL_FACTOR, VEL_MIN, VEL_MAX);
			t.updateSimulated();
		}
		//Cambiar masa real del proyectil
		if (key == 'g' || key == 'G') {
			t.rMass = physx::PxClamp(t.rMass * VAL_FACTOR, MASS_MIN, MASS_MAX);
			t.updateSimulated();
		}
		if (key == 'h' || key == 'H') {
			t.rMass = physx::PxClamp(t.rMass / VAL_FACTOR, MASS_MIN, MASS_MAX);
			t.updateSimulated();
		}

		if (key == 'p' || key == 'P') t.showDebug();           // tipo seleccionado
		if (key == 'i' || key == 'I') {						   // todos
			// --IMP:  no olvidar luego que, hacer vector de limpieza o muertos o lo que sea necesario en caso de implementacion de pool
			for (Particle* p : _projs) {
				if (p)
					p->showParamsDebug();
			}
		}
	}

	void cleanup() override {
		for (Particle* p : _projs) {
			delete p;p = nullptr;
		}
		_projs.clear();
		_projsTypes.clear();
	}
private:
	void shoot(const ProjectileType& t) {
		//Obtener la camara y sus params
		const Camera* cam = GetCamera();
		assert(cam != nullptr);
		const Vector3D camDir(cam->getDir());   // ya normalizada
		const Vector3D camPos(cam->getEye());

		//Calculamos la velocidad orientada hacia donde apunta la camara
		const Vector3D velFromCamera = t.sVel * camDir;
		const Vector3D acc(0.f, t.sGrav, 0.f);

		//Generar proyectil
		_projs.emplace_back(new Projectile(camPos, velFromCamera, acc, t.sMass, t.damping, t.size, t.color));
	};

private:
	std::vector<Particle*> _projs; // En la pract pone vector de particulas
	std::vector<ProjectileType> _projsTypes;
	size_t _sel = 0;

	static constexpr float VAL_FACTOR = 3.0f / 4.0f;
	static constexpr float VEL_MIN = 1.0f;
	static constexpr float VEL_MAX = 10000.0f;
	static constexpr float MASS_MIN = 0.0001f;
	static constexpr float MASS_MAX = 10000.0f;
};