#include "particlepopulation.h"
#include <stdlib.h>

ParticlePopulation::ParticlePopulation(int size, OptimizationFunction *optFunc)
    :Population(size, optFunc)
{
    mPrevBestValues=new double[mSize];
    mPrevBestPositions=new double[mSize * mDim];
}

ParticlePopulation::~ParticlePopulation()
{
    delete[] mPrevBestValues;
    delete[] mPrevBestPositions;
}

void ParticlePopulation::evaluatePopulation()
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


void ParticlePopulation::initializePopulation(double *range)
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




double ParticlePopulation::getBestValueFound() const { return mPrevBestValues[mBestPointIndex]; }
double* ParticlePopulation::getBestPositionFoundPointer() const { return &(mPrevBestPositions[mDim * mBestPointIndex]); }

double ParticlePopulation::getPrevBestValue(int individualIndex) const { return mPrevBestValues[individualIndex]; }
double* ParticlePopulation::getPrevBestPositionPointer(int individualIndex) const { return &(mPrevBestPositions[mDim * individualIndex]); }







