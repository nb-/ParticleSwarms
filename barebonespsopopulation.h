#ifndef BAREBONESPSOPOPULATION_H
#define BAREBONESPSOPOPULATION_H

//==================================================================
//    Implementation of the BareBones Particle Swarm
//==================================================================

#include "population.h"

class BareBonesPSOPopulation : public Population
{
protected:
    double mPAccel;
    double mGAccel;

public:
    BareBonesPSOPopulation(int size, double pAccel, double gAccel, OptimizationFunction* optFunc);
    virtual ~BareBonesPSOPopulation();
    virtual void updatePopulation();
};

#endif // BAREBONESPSOPOPULATION_H
