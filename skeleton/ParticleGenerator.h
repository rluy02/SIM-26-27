#pragma once
#include <random>
#include "Vector3D.h"
#include "SimulationConfig.h"
#include "RenderUtils.hpp"

// Lo que es comun al emisor (color, damping) va aparte
struct ParticleSample {
	Vector3D pos;
	Vector3D vel;
	float life;
};

struct EmitterConfig {
	Vector3D pos{ 0.f, 0.f, 0.f }, posSigma{ 0.f, 0.f, 0.f };  // media y desviacion por eje (>= 0)
	Vector3D vel{ 0.f, 0.f, 0.f }, velSigma{ 0.f, 0.f, 0.f };
	Vector3D gravity = Vector3D(0.f, simulation::systemWorldGravityY, 0.f); //por default la gravedad base
	float lifeMin = 1.f, lifeMax = 3.f;                        // segundos. Uniforme en [lifeMin, lifeMax)
	float meanPerFrame = 1.f, varPerFrame = 0.f;
	float damping = 1.f;
	float size = 0.5f;                                         // constante por emisor (esfera compartida)
	Vector4 color{ 1.f, 1.f, 1.f, 1.f };
};

/// <summary>
/// Emisor, guarda las configuraciones y produce los valores iniciales
/// </summary>
class ParticleGenerator {
public:
	explicit ParticleGenerator(const EmitterConfig& cfg);
	size_t howMany(float lodFactor = 1.f);
	ParticleSample sample();    // valores iniciales
	const EmitterConfig& config() const { return _config; }
private:
	Vector3D gaussian(const Vector3D& mean, const Vector3D& sigma); // 3 sorteos independientes
	EmitterConfig _config;
	std::mt19937 _gen;
	std::normal_distribution<float>       _N{ 0.f, 1.f };
	std::uniform_real_distribution<float> _U{ -1.f, 1.f }; //Segun diapos U(-1,1) para uniformidad

};