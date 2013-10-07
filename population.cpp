#include "population.h"

Population::Population(int size, OptimizationFunction* optFunc)
   :mSize(size)
{
    mGenNum = 0;
    mOptFunc = optFunc; // Population does NOT manage opt function memory! do not delete
    mBounds = optFunc->getBounds(); //Population does NOT manage bounds memory!
    mDim = optFunc->getDim();
    mValues = new double[mSize];
    mPositions=new double[mSize * mDim];
    mBestPointIndex=0;
}

Population::~Population()
{
    delete[] mPositions;
    delete[] mValues;
}

double Population::getLowerBound(int dim) const
{
    if(dim < mDim && dim >= 0)
        return mBounds[dim];
    else
        return 0;
}
double Population::getUpperBound(int dim) const
{
    if(dim < mDim && dim >= 0)
        return mBounds[dim + mDim];
    else
        return 0;
}
void Population::step()
{
    ++mGenNum;
    updatePopulation();
}
int Population::getPopSize() const { return mSize; }
int Population::getDim() const { return mDim; }

int Population::getGenerationNumber() const { return mGenNum; }

OptimizationFunction *Population::getOptimizationFunction() const { return mOptFunc; }

int Population::getBestIndividualIndex() const { return mBestPointIndex; }

double Population::getValue(int individualIndex) const { return mValues[individualIndex]; }
double* Population::getPositionPointer(int individualIndex) const { return &(mPositions[mDim * individualIndex]); }


