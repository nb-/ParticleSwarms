#include "population.h"
#include <stdlib.h>

Population::Population(int size, OptimizationFunction *optFunc)
   :mSize(size)
{
    mGenNum = 0;
    mOptFunc = optFunc; // Population does NOT manage opt function memory! do not delete
    mBounds = optFunc->getBounds(); //Population does NOT manage bounds memory!
    mDim = optFunc->getDim();
    mValues = new double[mSize];
    mPositions=new double[mSize * mDim];
    mPrevBestValues=new double[mSize];
    mPrevBestPositions=new double[mSize * mDim];
    mBestPointIndex=0;
}

Population::~Population()
{
    delete[] mPositions;
    delete[] mPrevBestPositions;
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

void Population::evaluatePopulation()
{
    int maxIt;

    for(int i = 0 ; i < mSize ; ++i)
        mOptFunc->evaluate(&(mPositions[i * mDim]), mValues[i]);

    for(int i = 0 ; i < mSize ; ++i)
    {
        if(mValues[i] < mPrevBestValues[i])
        {
            if(mValues[i] < mPrevBestValues[mBestPointIndex])
                mBestPointIndex = i;
            mPrevBestValues[i] = mValues[i];
            maxIt = i * mDim + mDim;
            for(int j = i * mDim ; j < maxIt ; ++j)
                mPrevBestPositions[j] = mPositions[j];
        }
    }
}


void Population::initializePopulation(double *range)
{

    if(range==0)
        range = mBounds;
    //add null checks for all things

    mGenNum = 0;

    int i = 0;
    int j = 0;
    int maxIt = mSize * mDim;

    while(i < maxIt)
    {
        mPositions[i] = range[j] + ((((double)rand()/(double)RAND_MAX) * (range[mDim + j] - range[j])));
        mPrevBestPositions[i] = mPositions[i];
        ++i;
        ++j;
        if(j==mDim) j = 0;
    }
    evaluatePopulation();
    for(i = 0 ; i < mSize ; ++i)
    {
        mPrevBestValues[i] = mValues[i];
        if(mPrevBestValues[i] < mPrevBestValues[mBestPointIndex])
            mBestPointIndex=i;
    }

}


int Population::getPopSize() const { return mSize; }
int Population::getDim() const { return mDim; }

int Population::getGenerationNumber() const { return mGenNum; }

OptimizationFunction *Population::getOptimizationFunction() const { return mOptFunc; }

int Population::getBestIndividualIndex() const { return mBestPointIndex; }
double Population::getBestValueFound() const { return mPrevBestValues[mBestPointIndex]; }
double* Population::getBestPositionFoundPointer() const { return &(mPrevBestPositions[mDim * mBestPointIndex]); }

double Population::getValue(int individualIndex) const { return mValues[individualIndex]; }
double* Population::getPositionPointer(int individualIndex) const { return &(mPositions[mDim * individualIndex]); }

double Population::getPrevBestValue(int individualIndex) const { return mPrevBestValues[individualIndex]; }
double* Population::getPrevBestPositionPointer(int individualIndex) const { return &(mPrevBestPositions[mDim * individualIndex]); }







