#include "ParticleGenerator.h"
#include <algorithm>

ParticleGenerator::ParticleGenerator(const EmitterConfig& cfg)
	: _config(cfg), _gen(std::random_device{}()) {
}

Vector3D ParticleGenerator::gaussian(const Vector3D& mean, const Vector3D& sigma) {
	//Cada llamada al generador es diferente
	Vector3D m;
	m.x = mean.x + sigma.x * _N(_gen);
	m.y = mean.y + sigma.y * _N(_gen);
	m.z = mean.z + sigma.z * _N(_gen);
	return m;
}

size_t ParticleGenerator::howMany(float lodFactor) {
	//NPartsf = max(0, ⌊MeanPartsf + U · VarPartsf ⌉) // distribucion simetrica
	const float n = (_config.meanPerFrame + _U(_gen) * _config.varPerFrame) * lodFactor;
	return static_cast<size_t>(std::fmax(0.f, std::round(n)));

	//ScreenSizeFactor = clamp(︄R/(d · tan(FOV/2)),0,0,1,0)
	//agregar en cuanto sea posible
}

ParticleSample ParticleGenerator::sample() {
	ParticleSample s;
	s.pos = gaussian(_config.pos, _config.posSigma);
	s.vel = gaussian(_config.vel, _config.velSigma);
	std::uniform_real_distribution<float> dist_life(_config.lifeMin, _config.lifeMax); //[,] en float
	s.life = dist_life(_gen);
	return s;
}