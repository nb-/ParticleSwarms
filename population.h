#ifndef POPULATION_H
#define POPULATION_H


//==================================================================
//    Base Class for Population Based BB Optimizer | Abstract
//==================================================================

#include "optimizationFunctions.h"

class Population
{
protected:
    const int mSize;
    int mDim;

    OptimizationFunction* mOptFunc;

    int mGenNum;

    int mBestPointIndex;
    double* mBounds;
    double* mValues;
    double* mPositions;

public:
    Population(int size, OptimizationFunction* optFunc);
    virtual ~Population();

    OptimizationFunction* getOptimizationFunction() const;

    int getPopSize() const;
    int getDim() const;
    int getGenerationNumber() const;

    int getBestIndividualIndex() const;

    virtual double getBestValueFound() const = 0;
    virtual double* getBestPositionFoundPointer() const = 0;

    double  getValue(int individualIndex) const;
    double* getPositionPointer(int individualIndex) const;

    double getLowerBound(int dim) const;
    double getUpperBound(int dim) const;

    void step();

    virtual void initializePopulation(double* range = 0) = 0;
    virtual void evaluatePopulation() = 0;
    virtual void updatePopulation() = 0;
};

#endif // POPULATION_H
