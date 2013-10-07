#ifndef NEWDEV1POPULATION_H
#define NEWDEV1POPULATION_H

//==================================================================
//  First Implemented Idea
//      Fips Square weighed by value
//      assign pool sizes randomly based on DParticle value
//      generate within gauss with fips Pm
//      update DParticle to best particle found
//==================================================================

#include "population.h"

class NewDev1Population : public Population
{
protected:
    const int mDSize;   //'Distribution' Particles
    double* mDValues;   //Similar to PrevBest, but act differently
    double* mDPositions;

    int* mDNeighbours; //FIPS neighbourhood, Square

    double mMaxPhi;

public:
    NewDev1Population(int pointPopSize, int dPopSize, double maxPhi, OptimizationFunction* optFunc);
    virtual ~NewDev1Population();

    int getDSize() const;

    virtual double getBestValueFound() const;
    virtual double* getBestPositionFoundPointer() const ;

    double  getDValue(int individualIndex) const;
    double* getDPositionPointer(int individualIndex) const;

    virtual void initializePopulation(double* range = 0);
    virtual void evaluatePopulation();
    virtual void updatePopulation();
};

#endif // NEWDEV1POPULATION_H
