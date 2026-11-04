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
#include "../ParticleSystemWithForces.h"
#include "../Generators/ExplosionGenerator.h"
#include "../Generators/ForceGenerator.h"
#include "../Generators/WhirlwindGenerator.h"

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
		p1.pSize = 0.5f;

		p2.pos = Vector3D(-15.f, 0.f, 0.f);
		p2.vel = Vector3D(0.f, 0.f, -6.f);
		p2.mass = 1.5f;
		p2.pSize = 2.f;
		p2.color = Vector4(0.3f, 0.7f, 0.2f, 1.f);

		apartadoA(p1, p2);											// comentar-descomentar esta linea para ver el resultado
		//---------------------------------------------------
		//------------ Apartado B (configurable) ------------
		// Nombre, masa real, velocidad real, gravedad real, velocidad simulada, damping, tamaño, color
		ProjectileType pt1("Bala", 0.0075f, 340.f, simulation::worldGravityY, 30.f, 1.f, 2.f, Vector4(1.f, 0.f, 0.f, 1.f));
		ProjectileType pt2("Bola de canon", 5.f, 300.f, simulation::worldGravityY, 40.f, 1.f, 6.f, Vector4(0.f, 1.f, 0.f, 1.f));

		apartadoB(pt1, pt2);										    // comentar-descomentar esta linea para ver el resultado
		//---------------------------------------------------
		//------------ Apartado C (config apA) ------------
		apartadoC(p1, p2);										     	// comentar-descomentar esta linea para ver el resultado
		//---------------------------------------------------
		//------------ Apartado E (necesita apC) ------------
		apartadoE(p1.pSize);											// comentar-descomentar esta linea para ver el resultado	
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
		//---------------------------------------------------
		//-------------------- Apartado C -------------------
		for (const auto& p : _systems) {
			p->update(dt); //Ya se gestionan las fuerzas dentro del sistema
		}
		//---------------------------------------------------
		//-------------------- Apartado E -------------------
		if (_explosion)
			_explosion->update(dt);

	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		//-------------------- Apartado A -------------------
		if (key == 'i' || key == 'I')
			for (const auto& p : _particles) p->showParamsDebug();
		//---------------------------------------------------
		//-------------------- Apartado B -------------------
		if (!_projectileTypes.empty()) {
			if (key == 'c' || key == 'C') {								    // 'C' para cambiar el tipo de proyectil
				_selected = (_selected + 1) % _projectileTypes.size();
				_projectileTypes[_selected].showDebug();
			}
			if (key == ' ') {											    // 'Espacio'para disparar
				shoot(_projectileTypes[_selected]);
				return;
			}
			if (key == 'p' || key == 'P') {								    // 'P' para mostrar informacion del proyectil actual
				_projectileTypes[_selected].showDebug();
			}
		}
		//---------------------------------------------------
		//-------------------- Apartado D -------------------
		if (!_systems.empty()) {
			if (key == 'v' || key == 'V') {
				_selectedSystem = (_selectedSystem + 1) % _systems.size();	// 'V' cambia la seleccion del sistema de particulas
				std::cout << "SELECCION ACTUAL [ " << _systems[_selectedSystem]->getName() << " ]" << std::endl;
			}
			if (key == 'g' || key == 'G')
				_systems[_selectedSystem]->toggleEmission();				// 'G' activa/desactiva el generador de particulas en seleccion

			if (key == 'o' || key == 'O')
				_systems[_selectedSystem]->showDebug();						// 'O' muestra informacion del sistema actual
		}
		//---------------------------------------------------
		//-------------------- Apartado E -------------------
		if (!_systems.empty()) {
			if (key == 'e' || key == 'E')									// 'E' genera las explosion
				if (_explosion)
					_explosion->activate(); // despues de activarse, se desactiva con el pasar del tiempo

			if ((key == 'h' || key == 'H') && _whirl) {						// 'H' agrega/quita el generador de torbellinos solo en el sistema seleccionado
				const bool active = _systems[_selectedSystem]->toggleForceGenerator(_whirl);
				std::cout << "Torbellino " << (active ? "REGISTRADO" : "RETIRADO") << " en " << _systems[_selectedSystem]->getName() << std::endl;
			}
			if ((key == 'j' || key == 'J') && _explosion) {					// 'J' agrega/quita el generador de explosiones solo en el sistema seleccionado
				const bool active = _systems[_selectedSystem]->toggleForceGenerator(_explosion);
				std::cout << "Explosion " << (active ? "REGISTRADO" : "RETIRADA") << " en " << _systems[_selectedSystem]->getName() << std::endl;
			}
		}
	}

	void cleanup() override {
		//ApartadoA
		_particles.clear();
		//ApartadoB
		_projectileTypes.clear();
		_projectiles.clear();
		_selected = 0;
		//ApartadoC
		_systems.clear();
		_selectedSystem = 0;
		//ApartadoE
		_whirl = nullptr;
		_explosion = nullptr;
		_forceGenerators.clear();
	}

private:
#pragma region Apartado A 
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
#pragma endregion
#pragma region Apartado B 
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
	};
#pragma endregion
#pragma region Apartado C 
	void apartadoC(const Particle& p1, const Particle& p2) {
		//Se hace uso del modelo de las particulas creadas en el apartado A.
		EmitterConfig ec1;
		ec1.pos = p1.pos;
		ec1.vel = p1.vel;
		ec1.size = p1.pSize;
		ec1.color = p1.color;
		ec1.mass = p1.mass;
		ec1.damping = p1.damping;
		ec1.velSigma = Vector3D(1.f, 1.f, 1.f);
		ec1.posSigma = Vector3D(4.f, 4.f, 4.f);
		ec1.lifeMin = 3.5f; ec1.lifeMax = 6.5f;
		ec1.meanPerFrame = 3.f; ec1.varPerFrame = 1.f;

		EmitterConfig ec2;
		ec2.pos = p2.pos;
		ec2.vel = p2.vel;
		ec2.size = p2.pSize;
		ec2.color = p2.color;
		ec2.mass = p2.mass;
		ec2.damping = p2.damping;
		ec2.velSigma = Vector3D(2.f, 2.f, 2.f);
		ec2.posSigma = Vector3D(4.f, 4.f, 4.f);
		ec2.lifeMin = 5.5f; ec2.lifeMax = 10.5f;
		ec2.meanPerFrame = 3.f; ec2.varPerFrame = 1.f;
		//No se registra ningun generador de fuerzas todavia.
		_systems.emplace_back(std::make_unique<ParticleSystemWithForces>(ec1, "Sist. Azul"));
		_systems.emplace_back(std::make_unique<ParticleSystemWithForces>(ec2, "Sist. Verde"));
	}
#pragma endregion
#pragma region Apartado E
	template <typename T>
	T* addGenerator(std::unique_ptr<T> generator) {
		T* ptr = generator.get();
		_forceGenerators.push_back(std::move(generator));
		return ptr;
	}
	void apartadoE(float pSize) {
		// Torbellino
		auto* whirl = addGenerator(std::make_unique<WhirlwindGenerator>(Vector3D(0.f, 0.f, 0.f), pSize, 30.f, 30.f, 0.2f)); //Este afecta a ambos sistemas
		_whirl = whirl;
		//Explosion
		auto* explosion = addGenerator(std::make_unique<ExplosionGenerator>(Vector3D(0.f, 10.f, 0.f), 300.f, 130.f, 1.f)); // cerca de donde spawnea el sistema
		_explosion = explosion;

		// Se aprovechan los sistemas del apartado C
		if (_systems.empty()) return;
		// Sistema azul: recibe ambos generadores inicialmente
		_systems[0]->addForceGenerator(whirl);
		_systems[0]->addForceGenerator(explosion);

		// Sistema verde: comparte el torbellino.
		_systems[1]->addForceGenerator(whirl); //Se le puede agregar a traves de la seleccion y con 'J' el generador de explosion
	}
#pragma endregion
#pragma region Miembros clase
	//ApartadoA
	std::vector< std::unique_ptr<ParticleWithForces>> _particles;
	//ApartadoB
	std::vector<ActiveProjectile> _projectiles;
	std::vector<ProjectileType> _projectileTypes;
	int _selected = 0;
	//ApartadoC
	std::vector< std::unique_ptr<ParticleSystemWithForces>>_systems;
	//ApartadoD
	size_t _selectedSystem = 0;
	//ApartadoE
	std::vector<std::unique_ptr<ForceGenerator>> _forceGenerators;
	WhirlwindGenerator* _whirl = nullptr;
	ExplosionGenerator* _explosion = nullptr; //si se quisieran simular varias explosiones se implementaria un vector para recorrerlos
};
#pragma endregion