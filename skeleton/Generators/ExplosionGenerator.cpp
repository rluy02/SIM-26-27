#include <cmath>
#include "ExplosionGenerator.h"
#include "../ParticleWithForces.h"

ExplosionGenerator::ExplosionGenerator(const Vector3D& pos, float k, float radius, float constante) : _pos(pos),
_k(k), _radiusInfluence(radius), _constTiempo(constante), _elapsedTime(0.f), _active(false) {

}

void ExplosionGenerator::updateForce(ParticleWithForces* p, float dt) {
	assert(p != nullptr);
	assert(_radiusInfluence > 0.f); //evitamos dividir por 0
	assert(_constTiempo > 0.f);
	if (!_active)return;

	const Vector3D pPos = p->getPosition();
	const float dx = pPos.x - _pos.x;
	const float dy = pPos.y - _pos.y;
	const float dz = pPos.z - _pos.z;
	const float distSq = dx * dx + dy * dy + dz * dz;

	if (distSq >= _radiusInfluence * _radiusInfluence || distSq < 1e-4f) //tambien evitamos radios de efecto sobre particual<->explosion
		return;

	const float factorDistancia = _k / distSq;
	const float factorTiempo = std::exp(-_elapsedTime / _constTiempo);
	Vector3D force = factorDistancia * Vector3D(dx, dy, dz) * factorTiempo;
	p->addForce(force);
}

void ExplosionGenerator::update(double dt) {
	if (_active) {
		_elapsedTime += static_cast<float>(dt);

		if (_elapsedTime >= 4.f * _constTiempo) //de la diapo, es casi impercitible asi que lo apagamos para poder generar mas explosiones
			_active = false;
	}
}

void ExplosionGenerator::activate() {
	_elapsedTime = 0.f;
	_active = true;
};

//Se habla del tiempo DESDE que ocurrio la explosion. Con eso en cuenta se puede simular la perdida de energia