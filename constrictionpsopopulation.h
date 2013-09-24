#ifndef CONSTRICTIONPSOPOPULATION_H
#define CONSTRICTIONPSOPOPULATION_H

//==================================================================
//    Particle Swarm Optimization With Constriction Coefficient
//==================================================================

#include "particlevpopulation.h"

class ConstrictionPSOPopulation : public ParticleVPopulation
{
protected:
    double mConstrictionCoef;
    double mPAccel;
    double mGAccel;

    void setConstrictionCoef();

public:
    ConstrictionPSOPopulation(int size, int dim, double pAccel, double gAccel, void (*optFunc)(int dim, double *position, double &value), double *bounds);
    virtual ~ConstrictionPSOPopulation();

    double getConstrictionCoef() const;
    double getPAccel() const;
    double getGAccel() const;

    void setPAccel(double pAccel);
    void setGAccel(double gAccel);

    virtual void updatePopulation();
};

#endif // CONSTRICTIONPSOPOPULATION_H
