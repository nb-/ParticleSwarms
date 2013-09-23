#include "population.h"
#include <stdlib.h>

Population::Population(int size, int dim, void (*optFunc)(int dim, double *position, double &value), double *bounds)
   :mSize(size),
    mDim(dim)//,
    //mBounds(bounds),
    //mPointIndices(new int[mSize]),
    //mValues(new double[mSize]),
    //mPositions(new double[mSize * mDim]),
   // mPrevBestValues(new double[mSize]),
    //mPrevBestPositions(new double[mSize * mDim]),
    //mOptFunction(optFunc),
    //mBestPointIndex(0)
{
    mBounds = bounds;
    mValues = new double[mSize];
    mPositions=new double[mSize * mDim];
    mPrevBestValues=new double[mSize];
    mPrevBestPositions=new double[mSize * mDim];
    mOptFunction=optFunc;
    mBestPointIndex=0;
}

Population::~Population()
{
    delete[] mBounds;
    delete[] mPositions;
    delete[] mPrevBestPositions;
    delete[] mValues;
}

double Population::getLowerBound(int dim)
{
    if(dim < mDim && dim >= 0)
        return mBounds[dim];
    else
        return 0;
}
double Population::getUpperBound(int dim)
{
    if(dim < mDim && dim >= 0)
        return mBounds[dim + mDim];
    else
        return 0;
}

void Population::evaluatePopulation()
{
    int maxIt;

    for(int i = 0 ; i < mSize ; ++i)
        mOptFunction(mDim, &(mPositions[i * mDim]), mValues[i]);

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


void Population::initializePopulation(double *range)
{

    if(range==0)
        range = mBounds;
    //add null checks for all things

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


const int Population::getPopSize()  { return mSize; }
const int Population::getDim()      { return mDim; }

int Population::getBestIndividualIndex()            { return mBestPointIndex; }
double Population::getBestValueFound()              { return mValues[mBestPointIndex]; }
double* Population::getBestPositionFoundPointer()   { return mPositions + (sizeof(double) * mBestPointIndex); }

double Population::getValue(int individualIndex)            { return mValues[individualIndex]; }
double* Population::getPositionPointer(int individualIndex) { return mPositions + (sizeof(double) * individualIndex); }

double Population::getPrevBestValue(int individualIndex)            { return mPrevBestValues[individualIndex]; }
double* Population::getPrevBestPositionPointer(int individualIndex) { return mPrevBestPositions + (sizeof(double) * individualIndex); }







