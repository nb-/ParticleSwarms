#include "constrictionpsopopulation.h"
#include <stdlib.h>
#include <math.h>

ConstrictionPSOPopulation::ConstrictionPSOPopulation(int size, double pAccel, double gAccel, OptimizationFunction *optFunc)
    :ParticleVPopulation(size,optFunc)
{
    mPAccel = pAccel;
    mGAccel = gAccel;
    setConstrictionCoef();
}

ConstrictionPSOPopulation::~ConstrictionPSOPopulation()
{

}


double ConstrictionPSOPopulation::getConstrictionCoef() const {return mConstrictionCoef;}
double ConstrictionPSOPopulation::getPAccel() const {return mPAccel;}
double ConstrictionPSOPopulation::getGAccel() const {return mGAccel;}

void ConstrictionPSOPopulation::setPAccel(double pAccel)
{
    if(pAccel == mPAccel) return;
    mPAccel = pAccel;
    setConstrictionCoef();
}
void ConstrictionPSOPopulation::setGAccel(double gAccel)
{
    if(gAccel == mGAccel) return;
    mGAccel = gAccel;
    setConstrictionCoef();
}

void ConstrictionPSOPopulation::updatePopulation()
{
    int minJIt = mBestPointIndex * mDim;
    int i = 0;
    int j = minJIt;
    int k = 0;
    int maxJIt = minJIt + mDim;
    int maxIIt = mSize * mDim;

    while(i < maxIIt)
    {
        mVelocities[i]  =   (
                                mVelocities[i]
                                + ( ((double)rand()/(double)RAND_MAX) * mPAccel * (mPrevBestPositions[i] - mPositions[i]) )
                                + ( ((double)rand()/(double)RAND_MAX) * mGAccel * (mPrevBestPositions[j] - mPositions[i]) )
                            )
                            * mConstrictionCoef;

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

void ConstrictionPSOPopulation::setConstrictionCoef()
{
    double p = mPAccel + mGAccel;
    mConstrictionCoef = ((double)2) / abs( 2 -  p - sqrt( (p * p) - (4 * p)));
}
