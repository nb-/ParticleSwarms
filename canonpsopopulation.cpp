#include "canonpsopopulation.h"
#include <stdlib.h>

CanonPSOPopulation::CanonPSOPopulation(int size, int dim, double inertia, double pAccel, double gAccel, void (*optFunc)(int dim, double *position, double &value), double *bounds)
   :ParticleVPopulation(size,dim, optFunc, bounds)
{
    mInertia = inertia;
    mPAccel = pAccel;
    mGAccel = gAccel;
}
CanonPSOPopulation::~CanonPSOPopulation()
{

}

double CanonPSOPopulation::getInertia()   {   return mInertia;  }
double CanonPSOPopulation::getPAccel()    {   return mPAccel;   }
double CanonPSOPopulation::getGAccel()    {   return mGAccel;   }

void CanonPSOPopulation::setInertia(double inertia) {   mInertia = inertia; }
void CanonPSOPopulation::setPAccel(double pAccel)   {   mPAccel = pAccel;   }
void CanonPSOPopulation::setGAccel(double gAccel)   {   mGAccel = gAccel;   }


void CanonPSOPopulation::updatePopulation()
{
    int minJIt = mBestPointIndex * mDim;
    int i = 0;
    int j = minJIt;
    int maxJIt = minJIt + mDim;
    int maxIIt = mSize * mDim;

    while(i < maxIIt)
    {
        mVelocities[i]  = (mVelocities[i] * mInertia)
                        + ( ((double)rand()/(double)RAND_MAX) * mPAccel * (mPrevBestPositions[i] - mPositions[i]) )
                        + ( ((double)rand()/(double)RAND_MAX) * mGAccel * (mPrevBestPositions[j] - mPositions[i]) ) ;
        mPositions[i]   += mVelocities[i];
        ++i;
        ++j;
        if(j==maxJIt) j = minJIt;
    }

    evaluatePopulation();

}
