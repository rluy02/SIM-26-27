#pragma once
#include <array>
#include "ParticleGenerator.h" 
/// <summary>
/// Dueño de la memoria, de actualizar y borrar
/// </summary>
class ParticleSystem {
public:
	explicit ParticleSystem(const EmitterConfig& cfg);
	~ParticleSystem();
	void update(double t);
private:
	void generateParticles(); //emit
	void spawn(const ParticleSample& s);
	void kill(size_t idx); //swap and pop
private:
	//comunes al emisor
	ParticleGenerator _particleGen;
	float _dampingFactor;     // pow(damping, fixedTimestep), calculado una vez
	Vector3D _centerEmitter;
	Vector4 _color;
	physx::PxShape* _shape;

	static constexpr size_t _MAX_PARTICLES = 5000;
	static constexpr float RADIUS_LIMIT = 10.f;
	static constexpr float  RADIUS_LIMIT_SQ = RADIUS_LIMIT * RADIUS_LIMIT; //para evitar hacer calculos de raiz cuadrada
	static constexpr float LIFE_TIME_LIMIT = 0.f;

	// por particula (SoA)
	size_t _alive = 0; // vivas en [0, _alive)
	std::array<physx::PxTransform, _MAX_PARTICLES> _pos;
	std::array<Vector3D, _MAX_PARTICLES> _vel;
	std::array<Vector3D, _MAX_PARTICLES> _acc;
	std::array<float, _MAX_PARTICLES> _life;
	std::array<RenderItem*, _MAX_PARTICLES> _items{}; //importante que no sea nullptr y cuando se itera que < alive
};

//Para mantener una estructura contigua, los vectores son implicitos a la clase, no a la particula en sí
//Modelo Paricula (le pasamos todo al generador)
//Interesa que varie la distribucion en los atributos: Pos(x,y,z), Vel(x,y,z), color
//gravedad -10 para todas las particulas

//NPartsf = MeanPartsr + distribucion x VarPartsr, 