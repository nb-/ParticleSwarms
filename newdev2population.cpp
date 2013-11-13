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
    mDPositions = new double[mDSize * mDim];
}

NewDev2Population::~NewDev2Population()
{
    delete[] mDValues;
    delete[] mDPositions;
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


}
void NewDev2Population::evaluatePopulation()
{

}

void NewDev2Population::updatePopulation()
{
    std::pair<double, int> temp;
    mValSort.clear();
    for(int i = 0 ; i < mDSize ; ++i)//todo: figure out how to modify data without having to readd all
    {
        temp.first = mDValues[i];
        temp.second = i;
        mValSort.push_back( temp );
    }
    std::sort(mValSort.begin(), mValSort.end()); //sort dParticles based on value

    int* mNeighbours = new int[mDSize];

    temp = mValSort.at(0);
    //mNeighbours[0] = temp.second;
    for(int i = 0 ; i < mDSize ; ++i)
    {
        //temp = mValSort.at( rand() % ( i ) );
        mNeighbours[i] = mBestPointIndex;
    }

    int* poolIndices = new int[mDSize + 1]; //used to assign each DParticle a number of Points

    int minimumPoolSize = 1;
    int pool = mSize - (minimumPoolSize * mDSize);
    int tempInt;
    std::vector<int> tempVec;

    for(int i = 0 ; i < mDSize ; ++i)
    {
        tempInt = ceil((double)pool / (double)(mDSize / 2));
        if(tempInt>1)
            tempInt = (rand() % (tempInt - 1)) + 1;
        tempVec.push_back(tempInt);
        pool -= tempInt;
    }
    while(pool > 0)
    {
        tempInt = ceil((double)pool / (double)(mDSize / 2));
        if(tempInt>1)
            tempInt = (rand() % (tempInt - 1)) + 1;
        tempVec.at( rand() % mDSize ) += tempInt;
        pool -= tempInt;
    }
    std::sort(tempVec.begin(), tempVec.end());


    poolIndices[0] = 0;
    for(int i = 1 ; i < mDSize ; ++i)
    {
        poolIndices[i] = poolIndices[i-1] + tempVec.at(mDSize - i) + 1;
    }
    poolIndices[mDSize] = mSize;

    tempVec.clear();

    double tempVal;
    double* tempPm = new double[mDim];
    int m;
    for(int i = 0 ; i < mDSize ; ++i) //Generate points
    {
        temp = mValSort.at(i);
        m = temp.second;
        for(int j = 0 ; j < mDim ; ++j)
        {
            tempPm[j] = (mDPositions[(m * mDim) + j] + mDPositions[( mNeighbours[i] * mDim ) + j]) / 2;
            tempVal = mDPositions[(m * mDim) + j] - mDPositions[( mNeighbours[i] * mDim ) + j];
            if(m == mBestPointIndex){
                tempVal = mDPositions[(m * mDim) + j] - mDPositions[( (mValSort.at(1)).second * mDim ) + j];
            }
                /*
            if(i == 0)//change?
                tempVal = mDPositions[(m * mDim) + j] - mDPositions[( mNeighbours[1] * mDim ) + j];
            if(tempVal == 0)
                tempVal = (mBounds[mDim + j] - mBounds[j])/10;
*/
            //todo: decide how to do the "local search" for best point

            for( int k = poolIndices[i] ; k < poolIndices[i + 1] ; ++k)
            {
                mPositions[(k * mDim) + j] = tempPm[j] + (randGauss() * tempVal);
                //mPositions[(k * mDim) + j] = tempPm[j] + (randDoubleExp() * tempVal);
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
        temp = mValSort.at(i);
        m = temp.second;
        tempVal = ((double)rand()/(double)RAND_MAX) * mDSize;
        bestInd = -1;
        if(true)//if(tempVal > i) //choose best
        {
            for( int k = poolIndices[i] ; k < poolIndices[i + 1] ; ++k)
            {
                if(mValues[k] < mDValues[m])
                {
                    if(mValues[k] < mDValues[mBestPointIndex])
                        mBestPointIndex = m;
                    bestInd = k;
                    mDValues[m] = mValues[k];
                }
            }
        }
        else            //choose random
        {
            for( int k = poolIndices[i] ; k < poolIndices[i + 1] ; ++k)
            {   //if a new global best is found, choose that regardless
                if(mValues[k] < mDValues[mBestPointIndex])
                {
                    mBestPointIndex = m;
                    bestInd = k;
                    mDValues[m] = mValues[k];
                }
            }
            if(bestInd == -1)
            {
                bestInd = (rand() % (poolIndices[i+1] - poolIndices[i])) + poolIndices[i];//random position
                mDValues[m] = mValues[bestInd];
            }
        }
        if(bestInd != -1)
        {
            for(int j = 0 ; j < mDim ; ++j)
            {
                mDPositions[(m*mDim) + j] = mPositions[(bestInd * mDim) + j];
            }
        }

    }

    delete[] mNeighbours;
    delete[] poolIndices;
}

