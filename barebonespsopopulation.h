#ifndef BAREBONESPSOPOPULATION_H
#define BAREBONESPSOPOPULATION_H

//==================================================================
//    Implementation of the BareBones Particle Swarm
//==================================================================

#include "population.h"

class BareBonesPSOPopulation : public Population
{
public:
    BareBonesPSOPopulation(int size, OptimizationFunction* optFunc);
    virtual ~BareBonesPSOPopulation();
    virtual void updatePopulation();
};

#endif // BAREBONESPSOPOPULATION_H
