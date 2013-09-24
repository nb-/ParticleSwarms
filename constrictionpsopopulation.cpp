#include "constrictionpsopopulation.h"
#include <stdlib.h>
#include <math.h>

ConstrictionPSOPopulation::ConstrictionPSOPopulation(int size, int dim, double pAccel, double gAccel, void (*optFunc)(int, double *, double &), double *bounds)
    :ParticleVPopulation(size,dim,optFunc,bounds)
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
        mPositions[i]   += mVelocities[i];
        ++i;
        ++j;
        if(j==maxJIt) j = minJIt;
    }

    evaluatePopulation();
}

void ConstrictionPSOPopulation::setConstrictionCoef()
{
    double p = mPAccel + mGAccel;
    mConstrictionCoef = ((double)2) / ( 2 -  p - sqrt( (p * p) - (4 * p)));
}
