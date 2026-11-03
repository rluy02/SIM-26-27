#pragma once

#include <vector>
#include <memory>
#include "Scene.h"
#include "../RenderUtils.hpp"
#include "../ParticleWithForces.h"

//Nota: Se hace uso de punteros inteligentes para reducir miembros en la clase y codigo extra en el cleanup
//Con comentar/descomentar en el init el "apartadoX" es suficiente para visualizar o quitar la implementacion que se desee
class MProject_Scene : public Scene {
public:
	explicit MProject_Scene(std::string name) : Scene(std::move(name)) {}

	void init() override {

		//------------ Apartado A (configurable) ------------
		Particle p1, p2;
		p1.pos = Vector3D(5.f, 0.f, 0.f);
		p1.vel = Vector3D(0.f, 0.f, -3.f);
		p1.mass = 0.5f;

		p2.pos = Vector3D(-5.f, 0.f, 0.f);
		p2.vel = Vector3D(0.f, 0.f, -6.f);
		p2.mass = 1.5f;

		apartadoA(p1, p2);							     //comentar-descomentar esta linea para ver el resultado
		//---------------------------------------------------
		//------------ Apartado B (configurable) ------------
		//---------------------------------------------------
	}

	void update(double dt) override {
		//-------------------- Apartado A -------------------
		for (const auto& p : _particles) {
			p->integrateVerlet(dt); //Con SEE tambien funciona
		}
		//---------------------------------------------------

	}

	void keyPress(unsigned char key, const physx::PxTransform& camera) override {
		if (key == 'i' || key == 'I')
			for (const auto& p : _particles) p->showParamsDebug();

	}

	void cleanup() override {
		_particles.clear();
	}

private:
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
		//Agregamos una fuerza inicial
		_particles[0]->addForce(Vector3D(0.f, 0.f, -5.f));
		_particles[1]->addForce(Vector3D(0.f, 0.f, -5.f));
	}
	//Metodo auxiliar para crear las particulas del apartado A
	std::unique_ptr<ParticleWithForces> createParticle(const Particle& p) {
		return std::make_unique<ParticleWithForces>(p.pos, p.vel, p.mass, p.damping, p.pSize, p.color);
	}
private:
	std::vector< std::unique_ptr<ParticleWithForces>> _particles;
};