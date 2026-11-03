#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <cassert>
#include <utility>
#include "Scene.h"
#include "../RenderUtils.hpp"
#include "../ParticleWithForces.h"
#include "../ProjectileWithForces.h"

//Nota: Se hace uso de punteros inteligentes para reducir miembros en la clase y codigo extra en el cleanup
//Con comentar/descomentar en el init el "apartadoX" es suficiente para visualizar o quitar la implementacion que se desee
class MProject_Scene : public Scene {
public:
	explicit MProject_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {

		//------------ Apartado A (configurable) ------------
		Particle p1, p2;
		p1.pos = Vector3D(15.f, 0.f, 0.f);
		p1.vel = Vector3D(0.f, 0.f, -3.f);
		p1.mass = 0.5f;

		p2.pos = Vector3D(-15.f, 0.f, 0.f);
		p2.vel = Vector3D(0.f, 0.f, -6.f);
		p2.mass = 1.5f;

		apartadoA(p1, p2);											// comentar-descomentar esta linea para ver el resultado
		//---------------------------------------------------
		//------------ Apartado B (configurable) ------------
		// Nombre, masa real, velocidad real, gravedad real, velocidad simulada, damping, tamaño, color
		ProjectileType pt1("Bala", 0.0075f, 340.f, simulation::worldGravityY, 30.f, 1.f, 2.f, Vector4(1.f, 0.f, 0.f, 1.f));
		ProjectileType pt2("Bola de canon", 5.f, 300.f, simulation::worldGravityY, 40.f, 1.f, 6.f, Vector4(0.f, 1.f, 0.f, 1.f));

		apartadoB(pt1, pt2);										// comentar-descomentar esta linea para ver el resultado
		//---------------------------------------------------
	}

	void update(double dt) override {
		//-------------------- Apartado A -------------------
		for (const auto& p : _particles) {
			p->clearForces();
			p->addForce(Vector3D(0.f, 0.f, -5.f)); //Se aplica la misma para ambas
			p->integrateVerlet(dt); //Con SEE tambien funciona
		}
		//---------------------------------------------------
		//-------------------- Apartado B -------------------
		for (auto& p : _projectiles) {
			p.particle->clearForces();
			p.particle->addForce(p.gravityForce);
			p.particle->integrateSEE(dt); //Ejemplo de que funciona correctamente con SEE
		}

	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		//-------------------- Apartado A -------------------
		if (key == 'i' || key == 'I')
			for (const auto& p : _particles) p->showParamsDebug();
		//---------------------------------------------------
		//-------------------- Apartado B -------------------
		if (!_projectileTypes.empty()) {
			if (key == 'c' || key == 'C') {								// 'C' para cambiar el tipo de proyectil
				_selected = (_selected + 1) % _projectileTypes.size();
				_projectileTypes[_selected].showDebug();
			}
			if (key == ' ') {											// 'Espacio'para disparar
				shoot(_projectileTypes[_selected]);
				return;
			}
			if (key == 'p' || key == 'P') {								// 'P' para mostrar informacion del proyectil actual
				_projectileTypes[_selected].showDebug();
			}
		}
		//---------------------------------------------------
	}

	void cleanup() override {
		//ApartadoA
		_particles.clear();
		//ApartadoB
		_projectileTypes.clear();
		_projectiles.clear();
		_selected = 0;
	}


private: //------------ Apartado A ------------
	struct Particle {
		Vector3D pos = Vector3D(0.f, 0.f, 0.f);
		Vector3D vel = Vector3D(0.f, 0.f, 0.f);
		float mass = 1.f;
		float damping = 1.00f;
		float pSize = 10.f;
		Vector4 color = Vector4(0.f, 1.f, 1.f, 1.f);

	};
	void apartadoA(const Particle& p1, const Particle& p2) {
		_particles.push_back(createParticle(p1));
		_particles.push_back(createParticle(p2));
	}
	//Metodo auxiliar para crear las particulas del apartado A
	std::unique_ptr<ParticleWithForces> createParticle(const Particle& p) {
		return std::make_unique<ParticleWithForces>(p.pos, p.vel, p.mass, p.damping, p.pSize, p.color);
	}

private: //------------ Apartado B ------------
	struct ProjectileType {
		std::string name;

		// Reales
		float rMass;
		float rVel;
		float rGrav;

		// Simulados
		float sVel;
		float sMass = 0.f;
		float sGrav = 0.f;
		float sForce = 0.f;

		float damping;
		float size;
		Vector4 color;

		ProjectileType(std::string n, float rm, float rv, float rg, float sv, float d, float s, const Vector4& c)
			: name(std::move(n)), rMass(rm), rVel(rv), rGrav(rg), sVel(sv), damping(d), size(s), color(c) {
			updateSimulated();
		}

		void updateSimulated() {
			assert(rVel != 0.f && sVel != 0.f);
			// Misma energia cinetica: Ms = Mr * Vr^2 / Vs^2
			sMass = rMass * (rVel * rVel) / (sVel * sVel);
			// Misma parabola: Gs = Gr * Vs^2 / Vr^2 (el signo de Gr se conserva)
			sGrav = rGrav * (sVel * sVel) / (rVel * rVel);
			// F = m * a
			sForce = sMass * sGrav;
		}

		void showDebug() const {
			std::cout << "[" << name << "] "
				<< "Masa: " << sMass
				<< " | Velocidad: " << sVel
				<< " | Gravedad: " << sGrav
				<< " | Fuerza: " << sForce << std::endl;
		}
	};
	struct ActiveProjectile {
		std::unique_ptr<ParticleWithForces> particle; //Sirve tanto 'ParticleWithForces' como 'ProjectileWithForces'. Replico la logica de P2S_Scene
		Vector3D gravityForce; //Con esto cada proyectil guarda su fuerza (parecido a lo que se aplica en los generadores)
	};

	void apartadoB(const ProjectileType& pt1, const ProjectileType& pt2) {
		_projectileTypes.push_back(pt1);
		_projectileTypes.push_back(pt2);

		_selected = 0;
	};

	void shoot(const ProjectileType& type) {
		const Camera* cam = GetCamera();
		assert(cam != nullptr);

		const Vector3D pos(cam->getEye());
		const Vector3D dir(cam->getDir());

		ActiveProjectile p;
		//Generar proyectil
		p.particle = std::make_unique<ProjectileWithForces>(pos, type.sVel * dir, type.sMass, type.damping, type.size, type.color); //herencia
		p.gravityForce = Vector3D(0.f, type.sForce, 0.f); //la de la gravedad

		_projectiles.push_back(std::move(p));
	}

private: //------------ Apartado C ------------

private: //----Miembros clase----
	//ApartadoA
	std::vector< std::unique_ptr<ParticleWithForces>> _particles;
	//ApartadoB
	std::vector<ActiveProjectile> _projectiles;
	std::vector<ProjectileType> _projectileTypes;
	int _selected = 0;
	//ApartadoC
};