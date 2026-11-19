#pragma once
#include "SpringForceGenerator.h"
#include <memory>
class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(float k, float restingLength, const Vector3D& anchorPos);
	~AnchoredSpringFG();
};

