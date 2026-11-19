#pragma once
#include "ForceGenerator.h"
#include "../SimulationConfig.h"

class GravityGenerator :public ForceGenerator {
public:
	explicit GravityGenerator(const Vector3D& gravity = Vector3D(0.f, simulation::worldGravityY, 0.f));
	void updateForce(ParticleWithForces* p, float dt) override;
public: //ImGui
	//void drawUI() override;
	const char* uiName() const override { return "Gravity"; }
private:
	Vector3D _gravity;
};