#ifndef PARTICLEVPOPULATION_H
#define PARTICLEVPOPULATION_H

//==================================================================
//    Particles With Velocities | Abstract
//==================================================================

#include "population.h"

class ParticleVPopulation : public Population
{
protected:
    double* mVelocities;
public:
    ParticleVPopulation(int size, int dim, void (*optFunc)(int dim, double *position, double &value), double *bounds);
    virtual ~ParticleVPopulation();

    double* getVelocityPointer(int individualIndex) const;

    virtual void updatePopulation() = 0;
    virtual void initializePopulation(double* range = 0);
};

#endif // PARTICLEVPOPULATION_H
