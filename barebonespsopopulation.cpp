#include "barebonespsopopulation.h"
#include "misc.h"
BareBonesPSOPopulation::BareBonesPSOPopulation(int size, double pAccel, double gAccel, OptimizationFunction *optFunc)
   :Population(size, optFunc),
    mPAccel(pAccel),
    mGAccel(gAccel)
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

    double rPA;
    double rGA;

    while(i < maxIIt)
    {
        rPA = ((double)rand()/(double)RAND_MAX) * mPAccel;
        rGA = ((double)rand()/(double)RAND_MAX) * mGAccel;

        mPositions[i]   =  ( (rPA * mPrevBestPositions[i] ) + ( rGA * mPrevBestPositions[j]) ) / (rPA + rGA);

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
