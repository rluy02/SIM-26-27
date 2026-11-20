#pragma once
#include "WindGenerator.h"
#include "../RenderUtils.hpp"

class WhirlwindGenerator : public WindGenerator {
public:
	explicit WhirlwindGenerator(const Vector3D& pos, float rad, float rInfl, float vs = 50.f, float k = 1.f);
	~WhirlwindGenerator() override;
	Vector3D getWindVelocity(const ParticleWithForces* p) const override;

public: //ImGui
	void drawUI() override;
	const char* uiName() const override { return "Whirlwind"; }
	void setGizmoVisible(bool v) override;
private:
	void setPosition(const Vector3D& pos);
	inline Vector3D getPosition() const { return _pos; };

	//Especie de Gizmos, solo es ayuda visual para la escena
	void rebuildVisual();
	void clearVisual();
	void addEdge(int i, const physx::PxVec3& center, const physx::PxVec3& halfExtents);
private:
	Vector3D _pos;
	float _radiusInfluence;
	float _k; //constante proporcial a la fuerza del torbellino
	float _verticalStop;//unidades de distancia a las que dejara de aplicarse velocidad vertical
	
	physx::PxTransform _edgeTransforms[12];
	RenderItem* _edgeItems[12] = {};
	bool _visible = true;
};