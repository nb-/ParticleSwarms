#include "newdev3population.h"
#include "randomhelper.h"
#include <cmath>

NewDev3Population::NewDev3Population(int pointPopSize, int dPopSize, OptimizationFunction *optFunc)
    :NewDev2Population(pointPopSize,dPopSize,optFunc)
{

}
void NewDev3Population::updatePopulation()
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

    int mNeighbour;
    for(int i = 0 ; i < mDSize ; ++i) //Generate points
    {
        mNeighbour = mBestPointIndex;
        tempD = 0; //holds euclidean distance between points
        for(int j = 0 ; j < mDim ; ++j)
        {
            tempD += pow(mDPositions[(i * mDim) + j] - mDPositions[(mNeighbour * mDim)+j],2);
        }


        if(tempD == 0)//assumption: chance of points sharing exact same spot microscopic
        {
            mNeighbour = (rand()%(mDSize-1) + mNeighbour)%(mDSize);
            for(int j = 0 ; j < mDim ; ++j)
            {
                tempD += pow(mDPositions[(i * mDim) + j] - mDPositions[(mNeighbour * mDim)+j],2);
            }
        }
        tempD = sqrt(tempD);
        for(int j = 0 ; j < mDim ; ++j)
        {
            tempPm[j] = (mDPositions[( mNeighbour * mDim ) + j]) - (mDPositions[(i * mDim) + j]);
        }


        for( int k = mPoolIndices[i] ; k < mPoolIndices[i + 1] ; ++k)
        {
            //tempD2 = randDouble();//determine first part of random dist - point between points
            tempD2 = (randGauss()) + 0.5;
            for(int j = 0 ; j < mDim ; ++j)
            {
                if(tempD == 0)
                {
                    mPositions[(k * mDim) + j] = mDPositions[(i * mDim) + j] + (tempPm[j] * tempD2) + (randGauss() * (tempVariance[j] / mDSize));
                }
                else
                {
                    mPositions[(k * mDim) + j] = mDPositions[(i * mDim) + j] + (tempPm[j] * tempD2) + (randGauss() * (tempD / mDim * 2));
                }


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


