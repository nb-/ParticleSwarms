#ifndef NEWDEV2POPULATION_H
#define NEWDEV2POPULATION_H

//==================================================================
//  Second Implemented Idea
//      Eval DParticles
//      Sort by val
//      Assign pool size based on ranking
//      Assign neighbour based on ranking
//          Best is special case, gauss range over itself somehow
//      generate points with own pos and neighbour
//      set point to best found
//==================================================================

#include <vector>
#include <stdlib.h>
#include "population.h"

class NewDev2Population : public Population
{
protected:
    const int mDSize;   //'Distribution' Particles
    double* mDValues;   //Similar to PrevBest, but act differently
    double* mDPositions;

    std::vector<std::pair<double,int> > mValSort;//for sorting, maintained for better performance

public:
    NewDev2Population(int pointPopSize, int dPopSize, OptimizationFunction* optFunc);
    virtual ~NewDev2Population();

    int getDSize() const;

    virtual double getBestValueFound() const;
    virtual double* getBestPositionFoundPointer() const ;

    double  getDValue(int individualIndex) const;
    double* getDPositionPointer(int individualIndex) const;

    virtual void initializePopulation(double* range = 0);
    virtual void evaluatePopulation();
    virtual void updatePopulation();
};

#endif // NEWDEV2POPULATION_H
