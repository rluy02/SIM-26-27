#include "ParticleSystemWithForces.h"
#include "ParticleWithForces.h"

ParticleSystemWithForces::ParticleSystemWithForces(const EmitterConfig& cfg)
	: _particleGen(cfg),
	_centerEmitter(cfg.pos) {
}


ParticleSystemWithForces::~ParticleSystemWithForces()
{
	for (size_t i = 0; i < _alive; ++i) {
		delete _particles[i];
		_particles[i] = nullptr;
	}
}

void ParticleSystemWithForces::generateParticles() {
	const size_t n = _particleGen.howMany();
	for (size_t i = 0; i < n; i++)
	{
		if (_alive < _MAX_PARTICLES)  spawn(_particleGen.sample());
	}
}

void ParticleSystemWithForces::spawn(const ParticleSample& s) {
	const EmitterConfig& cfg = _particleGen.config();

	_particles[_alive] = new ParticleWithForces(
		s.pos,
		s.vel,
		cfg.mass,
		cfg.damping,
		cfg.size,
		cfg.color
	);

	_life[_alive] = s.life;

	++_alive;
}

void ParticleSystemWithForces::kill(size_t i) {
	const size_t last = _alive - 1;

	delete _particles[i]; //memoria del new creado, ademas cada particula tenia su propio shape, se destruye consigo

	if (i != last) {
		_particles[i] = _particles[last];
		_life[i] = _life[last];
	}

	_particles[last] = nullptr;
	_life[last] = 0.f;

	--_alive; //"pop"
}

void ParticleSystemWithForces::update(double dt) {
	generateParticles();
	for (size_t i = 0; i < _alive;) {
		ParticleWithForces* p = _particles[i];
		_life[i] -= static_cast<float>(dt);

		//hacemos dist euclidea 2 formas: punto-centro para cada componente y dist= sumatorio de comp^2
		//esto es igual a calcular el vector desde el centro y hacer prod escalar consigo mismo.
		Vector3D v = p->getPosition() - _centerEmitter;

		if (_life[i] <= LIFE_TIME_LIMIT || v.dot(v) > RADIUS_LIMIT_SQ) {
			kill(i);
			continue;
		}

		p->clearForces(); //Limp fuerzas
		_registry.applyForces(p, static_cast<float>(dt)); //Calc y acumular fuerzas a los generadores correspondientes
		p->integrateSEE(dt); //integrar estado y actualizar pos,vels

		i++;
	}
}

void ParticleSystemWithForces::addForceGenerator(ForceGenerator* generator) {
	_registry.add(generator);
}