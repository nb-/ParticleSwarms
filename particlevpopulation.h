#ifndef PARTICLEVPOPULATION_H
#define PARTICLEVPOPULATION_H

//==================================================================
//    Particles With Velocities | Abstract
//==================================================================

#include "particlepopulation.h"

class ParticleVPopulation : public ParticlePopulation
{
protected:
    double* mVelocities;
public:
    ParticleVPopulation(int size, OptimizationFunction *optFunc);
    virtual ~ParticleVPopulation();

    double* getVelocityPointer(int individualIndex) const;

    virtual void updatePopulation() = 0;
    virtual void initializePopulation(double* range = 0);
};

#endif // PARTICLEVPOPULATION_H
