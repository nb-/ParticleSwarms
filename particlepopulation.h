#ifndef PARTICLEPOPULATION_H
#define PARTICLEPOPULATION_H


#include "population.h"

//==================================================================
//  Base Class for Particle Population Based BB Optimizer | Abstract
//==================================================================

//TODO: Abstract further, currently contains info only needed by PSO, should work for any pop based bb opt

class ParticlePopulation : public Population
{
protected:

    double* mPrevBestValues;
    double* mPrevBestPositions;


public:
    ParticlePopulation(int size, OptimizationFunction* optFunc);
    virtual ~ParticlePopulation();

    virtual double getBestValueFound() const;
    virtual double* getBestPositionFoundPointer() const;

    double  getPrevBestValue(int individualIndex) const;
    double* getPrevBestPositionPointer(int individualIndex) const;

    virtual void initializePopulation(double* range = 0);
    virtual void evaluatePopulation();
    virtual void updatePopulation() = 0;
};
#endif // PARTICLEPOPULATION_H
