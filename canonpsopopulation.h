#ifndef CANONPSOPOPULATION_H
#define CANONPSOPOPULATION_H

//==================================================================
//    Canonical Particle Swarm Optimizer
//==================================================================

#include "particlevpopulation.h"

class CanonPSOPopulation : public ParticleVPopulation
{
protected:
    double mInertia;
    double mPAccel;
    double mGAccel;

public:
    CanonPSOPopulation(int size, int dim, double inertia, double pAccel, double gAccel, void (*optFunc)(int dim, double *position, double &value), double *bounds);
    virtual ~CanonPSOPopulation();

    double getInertia() const;
    double getPAccel() const;
    double getGAccel() const;

    void setInertia(double inertia);
    void setPAccel(double pAccel);
    void setGAccel(double gAccel);

    virtual void updatePopulation();
};


#endif // CANONPSOPOPULATION_H
