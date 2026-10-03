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
	double rMass;    // kg   (real)
	double rVel;     // m/s  (real)
	double rGrav;    // m/s^2 (real; positiva = flota)
	double sVel;     // m/s  (la elegimos nosotros)
	double damping;
	float size;
	Vector4 color;
	double sMass = 0.0;
	double sGrav = 0.0;

	ProjectileType(std::string n, double rm, double rv, double rg, double sv, double d, float s, const Vector4& c)
		: name(std::move(n)), rMass(rm), rVel(rv), rGrav(rg), sVel(sv), damping(d), size(s), color(c) {
		updateSimulated();
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
		const Vector4 r(1, 0, 0, 1);
		const Vector4 g(0, 1, 0, 1);
		const Vector4 b(0, 0, 1, 1);
		//nombre, rMass, rVel, rGrav, sVel, damping
		_projsTypes.emplace_back("Bala", 0.0075, 340.0, simulation::worldGravityY, 30.0, 1.0, 2.f, r);
		_projsTypes.emplace_back("Bola de canon", 5.0, 300.0, simulation::worldGravityY, 40.0, 1.0, 6.f, g);
		_projsTypes.emplace_back("Globo de helio", 0.003, 5.0, 2.0, 5.0, 0.9, 12.f, b);
		_sel = 0;
	}

	void update(double dt) override {
		const size_t n = _projs.size();
		for (size_t i = 0u; i < n; i++) //mas seguro que el for each
		{
			_projs.at(i)->integrate(dt);
		}
	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {

		//Reducir o aumentar un cuarto
		const double ValFactor = 3.0 / 4.0;

		ProjectileType& t = _projsTypes[_sel]; //Por ref para poder cambiar sVel o rMass

		//cambiar tipo de proyectil
		if (key == 'c' || key == 'C') { _sel = (_sel + 1) % _projsTypes.size();t.showDebug();}

		if (key == ' ') { shoot(t); return; } // dispara el tipo seleccionado

		//Cambiar velocidad simulada del proyectil (SE EFECTUA PARA EL SIGUIENTE A GENERARSE)
		if (key == 'j' || key == 'J') {
			t.sVel = physx::PxClamp(t.sVel * ValFactor, VEL_MIN, VEL_MAX);
			t.updateSimulated();
		}
		if (key == 'k' || key == 'K') {
			t.sVel = physx::PxClamp(t.sVel / ValFactor, VEL_MIN, VEL_MAX);
			t.updateSimulated();
		}
		//Cambiar masa real del proyectil
		if (key == 'g' || key == 'G') {
			t.rMass = physx::PxClamp(t.rMass * ValFactor, MASS_MIN, MASS_MAX);
			t.updateSimulated();
		}
		if (key == 'h' || key == 'H') {
			t.rMass = physx::PxClamp(t.rMass / ValFactor, MASS_MIN, MASS_MAX);
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
		const Vector3D camDir(cam->getDir());   // ya normalizada
		const Vector3D camPos(cam->getEye());

		//Calculamos la velocidad orientada hacia donde apunta la camara
		const Vector3D velFromCamera = static_cast<float>(t.sVel) * camDir;
		const Vector3D acc(0.f, static_cast<float>(t.sGrav), 0.f);

		//Generar proyectil
		_projs.emplace_back(new Projectile(camPos, velFromCamera, acc, t.sMass, t.damping, t.size, t.color));
	};

private:
	std::vector<Particle*> _projs; // En la pract pone vector de particulas
	std::vector<ProjectileType> _projsTypes;
	size_t _sel = 0;

	static constexpr double VEL_MIN = 1.0;
	static constexpr double VEL_MAX = 10000.0;
	static constexpr double MASS_MIN = 0.0001;
	static constexpr double MASS_MAX = 10000.0;
};