#include "newdev2population.h"
#include "randomhelper.h"
#include <stdlib.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

NewDev2Population::NewDev2Population(int pointPopSize, int dPopSize, OptimizationFunction *optFunc)
   :Population(pointPopSize, optFunc),
    mDSize(dPopSize)
{
    mDValues = new double[mDSize];
    mDAPValues = new double[mDSize];
    mDPositions = new double[mDSize * mDim];
    mPoolIndices = new int[mDSize+1];
}

NewDev2Population::~NewDev2Population()
{
    delete[] mDValues;
    delete[] mDAPValues;
    delete[] mDPositions;
    delete[] mPoolIndices;
}

int NewDev2Population::getDSize() const { return mDSize; }

double NewDev2Population::getBestValueFound() const { return mDValues[mBestPointIndex]; }

double* NewDev2Population::getBestPositionFoundPointer() const { return &(mDPositions[mBestPointIndex * mDim]); }

double NewDev2Population::getDValue(int individualIndex) const { return mDValues[individualIndex]; }

double* NewDev2Population::getDPositionPointer(int individualIndex) const { return &(mDPositions[individualIndex * mDim]); }

void NewDev2Population::initializePopulation(double *range)
{
    if(range==0)
        range = mBounds;
    //add null checks for all things

    mGenNum = 0;

    int i = 0;
    int j = 0;
    int maxIt = mDSize * mDim;

    while(i < maxIt)
    {
        mDPositions[i] = range[j] + ((((double)rand()/(double)RAND_MAX) * (range[mDim + j] - range[j])));
        ++i;
        ++j;
        if(j==mDim) j = 0;
    }
    mValSort.clear();
    std::pair<double,int> temp;
    mBestPointIndex = 0;
    for(int i = 0 ; i < mDSize ; ++i)
    {
        mOptFunc->evaluate(&(mDPositions[i * mDim]), mDValues[i]);
        temp.first = mDValues[i];
        temp.second = i;
        mValSort.push_back( temp );
        if(mDValues[i]<mDValues[mBestPointIndex])
            mBestPointIndex = i;
    }


    i = 0;
    j = 0;
    maxIt = mSize * mDim;

    while(i < maxIt)    //TODO: Decide what to do here, this is potentially bad maybe?
    {
        mPositions[i] = 0;
        ++i;
    }

    for(int i = 0 ; i < mSize ; ++i)
        mValues[i] = 0;

    for(int i = 0 ; i < mDSize ; ++i)
        mDAPValues[i] = mDValues[i];

}
void NewDev2Population::evaluatePopulation()
{

}

void NewDev2Population::updatePopulation()
{
    //normalize dapVals
    int tempI = 0;
    double tempD = 0;
    double tempD2 = 0;
    for(int i = 0 ; i < mDSize ; ++i)
    {
        tempD += mDAPValues[i];
    }
    if(tempD == 0) tempD = 1;

    for(int i = 0 ; i < mDSize ; ++i)
    {
        mDAPValues[i] = mDAPValues[i] / tempD; //scale values to sum to 1
    }


    tempI = mSize - mDSize;//tempI = pool size
    mPoolIndices[0] = 0;
    for(int i = 1 ; i <= mDSize ; ++i)
    {
        mPoolIndices[i] = mPoolIndices[i-1] + floor(mDAPValues[i-1] * tempI) + 1;
    }
    mPoolIndices[mDSize] = mSize;

    double* tempPm = new double[mDim];
    double* tempMean = new double[mDim];
    double* tempVariance = new double[mDim];
    for(int i = 0 ; i < mDim ; ++i)
    {
        tempMean[i] = 0;
        for(int j = 0 ; j < mDSize ; ++j)
        {
            tempMean[i] += mDPositions[(j*mDim) + i];
        }
        tempMean[i] /= mDSize;
    }

    for(int i = 0 ; i < mDim ; ++i)
    {
        tempVariance[i] = 0;
        for(int j = 0 ; j < mDSize ; ++j)
        {
            tempVariance[i] += (mDPositions[(j*mDim) + i] - tempMean[i]) * (mDPositions[(j*mDim) + i] - tempMean[i]);
        }
        tempVariance[i] = sqrt(tempVariance[i]) / mDSize;
        //tempVariance[i] /= mDSize;
    }

    bool pointsEqual = false;
    for(int i = 0 ; i < mDSize ; ++i) //Generate points
    {
        pointsEqual = (i==mBestPointIndex);
        if(!pointsEqual)
        {
            for(int j = 0 ; j < mDim ; ++j)
            {
                if(!(mDPositions[(i * mDim) + j] == mDPositions[( mBestPointIndex * mDim ) + j]))
                {
                    pointsEqual = false;
                    break;
                }
            }
        }

        for(int j = 0 ; j < mDim ; ++j)
        {
            tempPm[j] = (mDPositions[(i * mDim) + j] + mDPositions[( mBestPointIndex * mDim ) + j]) / 2;

            tempD = mDPositions[(i * mDim) + j] - mDPositions[( mBestPointIndex * mDim ) + j];

            if(pointsEqual)
            {
                tempD = tempVariance[j];
            }

            for( int k = mPoolIndices[i] ; k < mPoolIndices[i + 1] ; ++k)
            {
                 mPositions[(k * mDim) + j] = tempPm[j] + (randGauss() * tempD);

                if(mPositions[(k * mDim) + j] > mBounds[mDim + j])
                {
                    mPositions[(k * mDim) + j] = mBounds[mDim + j];
                }
                else if(mPositions[(k * mDim) + j] < mBounds[j])
                {
                    mPositions[(k * mDim) + j] = mBounds[j];
                }

            }
        }
    }

    for(int i = 0 ; i < mSize ; ++i)// evaluate points
        mOptFunc->evaluate(&(mPositions[i * mDim]), mValues[i]);

    int bestInd;
    for(int i = 0 ; i < mDSize ; ++i) //update points
    {
        bestInd = -1;

        mDAPValues[i] = 0;

        for( int k = mPoolIndices[i] ; k < mPoolIndices[i + 1] ; ++k)
        {
            //mDAPValues[i] += mValues[k];
            if(mValues[k] < mDValues[i])
            {
                if(mValues[k] < mDValues[mBestPointIndex])
                    mBestPointIndex = i;
                bestInd = k;
                mDValues[i] = mValues[k];
                mDAPValues[i] = mDValues[i];
            }
        }
        //mDAPValues[i] /= (double)(mPoolIndices[i + 1]-mPoolIndices[i]);

        if(bestInd != -1)
        {
            for(int j = 0 ; j < mDim ; ++j)
            {
                mDPositions[(i*mDim) + j] = mPositions[(bestInd * mDim) + j];
            }

        }

    }


    delete[] tempPm;
    delete[] tempMean;
    delete[] tempVariance;
}

