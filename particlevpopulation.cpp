#include "particlevpopulation.h"
#include <stdlib.h>

ParticleVPopulation::ParticleVPopulation(int size, OptimizationFunction *optFunc)
   :ParticlePopulation(size, optFunc)
{
    mVelocities = new double[mSize * mDim];
}
ParticleVPopulation::~ParticleVPopulation()
{
    delete[] mVelocities;
}

double *ParticleVPopulation::getVelocityPointer(int individualIndex) const
{
    return &(mVelocities[mDim * individualIndex]);
}

void ParticleVPopulation::initializePopulation(double *range)
{
    ParticlePopulation::initializePopulation(range);

    if(range==0)
        range = mBounds;

    int i = 0;
    int j = 0;
    int maxIt = mSize * mDim;
    double temp;

    while(i < maxIt)
    {
        temp = abs((range[mDim + j] - range[j]));
        mVelocities[i] = (temp * -1) + ( ((double)rand()/(double)RAND_MAX) * (2 * temp));
        ++i;
        ++j;
        if(j==mDim) j = 0;
    }
}
