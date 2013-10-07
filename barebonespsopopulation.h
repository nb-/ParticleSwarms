#ifndef BAREBONESPSOPOPULATION_H
#define BAREBONESPSOPOPULATION_H

//==================================================================
//    Implementation of the BareBones Particle Swarm
//==================================================================

#include "particlepopulation.h"

class BareBonesPSOPopulation : public ParticlePopulation
{
public:
    BareBonesPSOPopulation(int size, OptimizationFunction* optFunc);
    virtual ~BareBonesPSOPopulation();
    virtual void updatePopulation();
};

#endif // BAREBONESPSOPOPULATION_H
