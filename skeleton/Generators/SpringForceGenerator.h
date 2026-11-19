#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator :public ForceGenerator
{
public:
    SpringForceGenerator(double k, double resting_length, ParticleWithForces* p);
    void updateForce(ParticleWithForces* p,float dt) override;
    inline void setK(double k) { _k = k; }
private:
    double _k; //Coef de elasticidad
    double _restingLength;
    ParticleWithForces* _otherP;
};

