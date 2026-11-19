#pragma once
#include "ForceGenerator.h"
class SpringForceGenerator :public ForceGenerator
{
public:
    SpringForceGenerator(float k, float resting_length, ParticleWithForces* other);
    void updateForce(ParticleWithForces* p,float dt) override;
    inline void setK(float k) { _k = k; }
    inline float getK()const { return _k; }
protected:
    ParticleWithForces* _otherP;
private:
    float _k; //Coef de elasticidad
    float _restingLength;

};

