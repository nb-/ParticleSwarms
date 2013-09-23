#ifndef PARTICLEVPOPULATION_H
#define PARTICLEVPOPULATION_H

#include "population.h"

class ParticleVPopulation : public Population
{
protected:
    double* mVelocities;
public:
    ParticleVPopulation(int size, int dim, void (*optFunc)(int dim, double *position, double &value), double *bounds);
    virtual ~ParticleVPopulation();


    virtual void updatePopulation() = 0;
    virtual void initializePopulation(double* range = 0);
};

#endif // PARTICLEVPOPULATION_H
