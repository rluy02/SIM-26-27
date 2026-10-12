#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const EmitterConfig& cfg)
	: _particleGen(cfg),
	_dampingFactor(static_cast<float>(std::pow(cfg.damping, simulation::fixedTimestepF))),
	_centerEmitter(cfg.pos),
	_color(cfg.color),
	_shape(CreateShape(physx::PxSphereGeometry(cfg.size))) {
}


ParticleSystem::~ParticleSystem() {
	for (size_t i = 0; i < _alive; ++i) _items[i]->release();
	_shape->release();
}

void ParticleSystem::generateParticles() {
	const size_t n = _particleGen.howMany();
	for (size_t i = 0; i < n; i++)
	{
		if (_alive < _MAX_PARTICLES)  spawn(_particleGen.sample());
	}
}

void ParticleSystem::spawn(const ParticleSample& s) {
	_pos[_alive] = physx::PxTransform(s.pos);;
	_vel[_alive] = s.vel;
	_acc[_alive] = _particleGen.config().gravity;
	_life[_alive] = s.life;
	_items[_alive] = new RenderItem(_shape, &_pos[_alive], _color);
	++_alive;
}

void ParticleSystem::kill(size_t i) {
	//swap
	const size_t last = _alive - 1;
	std::swap(_pos[i], _pos[last]);
	std::swap(_vel[i], _vel[last]);
	std::swap(_acc[i], _acc[last]);
	std::swap(_life[i], _life[last]);

	//liberar recursos del que se "eliminara"
	_items[last]->release();
	_items[last] = nullptr;

	--_alive; //pop
}

void ParticleSystem::update(double t) {
	const float dt = static_cast<float>(t);   // único cast
	assert(dt == simulation::fixedTimestepF);
	generateParticles();
	for (size_t i = 0; i < _alive;) {
		_life[i] -= dt;

		//hacemos dist euclidea 2 formas: punto-centro para cada componente y dist= sumatorio de comp^2
		//esto es igual a calcular el vector desde el centro y hacer prod escalar consigo mismo.
		Vector3D v = _pos[i].p - _centerEmitter;

		if (_life[i] <= LIFE_TIME_LIMIT || v.dot(v) > RADIUS_LIMIT_SQ) {
			kill(i);
			continue;
		}
		_vel[i] = (_vel[i] + dt * _acc[i]) * _dampingFactor;
		_pos[i].p = _pos[i].p + dt * _vel[i];
		++i;
	}
}