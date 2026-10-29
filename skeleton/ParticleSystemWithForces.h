#pragma once
#include <array>
#include "ParticleGenerator.h" 
#include "ParticleForceRegistry.h"

//Nota: En esta version se hace uso de AoS para aprovechar la impl. de la particula con fuerza y depurar facilmente
class ParticleSystemWithForces {
public:
	explicit ParticleSystemWithForces(const EmitterConfig& cfg);
	~ParticleSystemWithForces();

	void update(double t);
	void addForceGenerator(ForceGenerator* generator); //pueden ser muchos que se agregaran al registro
private:
	void generateParticles();
	void spawn(const ParticleSample& s);
	void kill(size_t idx);
private:
	ParticleGenerator _particleGen;
	Vector3D _centerEmitter;
	ParticleForceRegistry _registry;

	static constexpr size_t _MAX_PARTICLES = 5000;
	static constexpr float RADIUS_LIMIT = 50.f;
	static constexpr float  RADIUS_LIMIT_SQ = RADIUS_LIMIT * RADIUS_LIMIT;
	static constexpr float LIFE_TIME_LIMIT = 0.f;

	size_t _alive = 0;

	std::array<ParticleWithForces*, _MAX_PARTICLES> _particles{}; //AoS
	std::array<float, _MAX_PARTICLES> _life{};
};