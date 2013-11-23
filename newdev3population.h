#ifndef NEWDEV3POPULATION_H
#define NEWDEV3POPULATION_H

#include "newdev2population.h"

class NewDev3Population : public NewDev2Population
{
public:
    NewDev3Population(int pointPopSize, int dPopSize, OptimizationFunction *optFunc);
    virtual void updatePopulation();
};

#endif // NEWDEV3POPULATION_H
