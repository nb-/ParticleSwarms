#include "barebonespsopopulation.h"
#include "randomhelper.h"
BareBonesPSOPopulation::BareBonesPSOPopulation(int size, OptimizationFunction *optFunc)
   :ParticlePopulation(size, optFunc)
{
}

BareBonesPSOPopulation::~BareBonesPSOPopulation()
{

}

void BareBonesPSOPopulation::updatePopulation()
{
    int minJIt = mBestPointIndex * mDim;
    int i = 0;
    int j = minJIt;
    int k = 0;
    int maxJIt = minJIt + mDim;
    int maxIIt = mSize * mDim;

    while(i < maxIIt)
    {
        mPositions[i]   =  ( (mPrevBestPositions[i] ) + (mPrevBestPositions[j]) ) / 2;

        mPositions[i] += randGauss() * (mPrevBestPositions[j] - mPrevBestPositions[i]);

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
