#include "canonpsopopulation.h"
#include <stdlib.h>

CanonPSOPopulation::CanonPSOPopulation(int size, double inertia, double pAccel, double gAccel, OptimizationFunction *optFunc)
   :ParticleVPopulation(size, optFunc)
{
    mInertia = inertia;
    mPAccel = pAccel;
    mGAccel = gAccel;
}
CanonPSOPopulation::~CanonPSOPopulation()
{

}

double CanonPSOPopulation::getInertia() const {return mInertia;}
double CanonPSOPopulation::getPAccel() const {return mPAccel;}
double CanonPSOPopulation::getGAccel() const {return mGAccel;}

void CanonPSOPopulation::setInertia(double inertia) {   mInertia = inertia; }
void CanonPSOPopulation::setPAccel(double pAccel)   {   mPAccel = pAccel;   }
void CanonPSOPopulation::setGAccel(double gAccel)   {   mGAccel = gAccel;   }


void CanonPSOPopulation::updatePopulation()
{
    int minJIt = mBestPointIndex * mDim;
    int i = 0;
    int j = minJIt;
    int k = 0;
    int maxJIt = minJIt + mDim;
    int maxIIt = mSize * mDim;

    while(i < maxIIt)
    {
        mVelocities[i]  = (mVelocities[i] * mInertia)
                        + ( ((double)rand()/(double)RAND_MAX) * mPAccel * (mPrevBestPositions[i] - mPositions[i]) )
                        + ( ((double)rand()/(double)RAND_MAX) * mGAccel * (mPrevBestPositions[j] - mPositions[i]) ) ;


        if(mVelocities[i] > (mBounds[mDim + k] - mBounds[k]))
        {
            mVelocities[i] = (mBounds[mDim + k] - mBounds[k]);
        }
        else if(mVelocities[i] < (mBounds[k] - mBounds[mDim + k]))
        {
            mVelocities[i] = (mBounds[k] - mBounds[mDim + k]);
        }

        mPositions[i]   += mVelocities[i];

        if(mPositions[i] > mBounds[mDim + k])
        {
            mPositions[i] = mBounds[mDim + k];
        }
        else if(mPositions[i] < mBounds[k])
        {
            mPositions[i] = mBounds[k];
        }
        ++i;
        ++j;
        ++k;
        if(j==maxJIt) j = minJIt;
        if(k==mDim) k = 0;
    }

    evaluatePopulation();

}
