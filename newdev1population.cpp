#include "newdev1population.h"
#include "randomhelper.h"
#include <cmath>

NewDev1Population::NewDev1Population(int pointPopSize, int dPopSize, double maxPhi, OptimizationFunction *optFunc)
   :Population(pointPopSize, optFunc),
    mDSize(dPopSize),
    mMaxPhi(maxPhi)
{
    mDValues = new double[mDSize];
    mDPositions = new double[mDSize * mDim];
    mDNeighbours = new int[mDSize * 4];
}

NewDev1Population::~NewDev1Population()
{
    delete[] mDValues;
    delete[] mDPositions;
    delete[] mDNeighbours;
}

int NewDev1Population::getDSize() const { return mDSize; }

double NewDev1Population::getBestValueFound() const { return mDValues[mBestPointIndex]; }

double* NewDev1Population::getBestPositionFoundPointer() const { return &(mDPositions[mBestPointIndex * mDim]); }

double NewDev1Population::getDValue(int individualIndex) const { return mDValues[individualIndex]; }

double* NewDev1Population::getDPositionPointer(int individualIndex) const { return &(mDPositions[individualIndex * mDim]); }

void NewDev1Population::initializePopulation(double *range)
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

    mBestPointIndex = 0;
    for(int i = 0 ; i < mDSize ; ++i)
    {
        mOptFunc->evaluate(&(mDPositions[i * mDim]), mDValues[i]);
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


    //square neighbourhood setup


    //
    //   9 > - +
    //   0 1 2 |
    // ^ 3 4 5 v
    // | 6 7 8 |
    // + - < - +
    //
    //

    int squareSide = (int)floor(sqrt(mDSize));
    int maxSquare = (int)ceil(sqrt(mDSize));

    int neighbourIt = 0;



    int* tempGrid = new int[maxSquare * maxSquare];
    for(i = 0 ; i < maxSquare ; ++i)
        for(j = 0 ; j < maxSquare ; ++j)
            tempGrid[(i * maxSquare) + j] = -1;

    for(int i = 0 ; i < squareSide ; ++i)
        for(int j = 0 ; j < squareSide ; ++j)
            tempGrid[((i+maxSquare - squareSide) * maxSquare) + j] = (i*squareSide) + j;

    int remainder = mDSize - (squareSide * squareSide);

    j = 0;
    i = 0;

    while(remainder > 0)
    {
        tempGrid[(i * maxSquare) + j] = mDSize - remainder;
        --remainder;
        if(j == maxSquare - 1)
        {
            ++i;
        }
        else
        {
            ++j;
        }
    }

    // 0 = left
    // 1 = top
    // 2 = right
    // 3 = bottom
    int k;
    for(i = 0 ; i < maxSquare ; ++i)
    {
        for( j = 0 ; j < maxSquare ; ++j)
        {
            if(tempGrid[(i*maxSquare) + j] != -1)
            {
                //search left
                k = (j==0) ? maxSquare-1 : j - 1;
                while( tempGrid[(i*maxSquare) + k] == -1 )
                    k = (k == 0) ? maxSquare - 1 : k - 1;
                mDNeighbours[tempGrid[(i*maxSquare) + j] * 4] = tempGrid[(i*maxSquare) + k];

                //search up
                k = (i==0) ? maxSquare-1 : i - 1;
                while( tempGrid[(k*maxSquare) + j] == -1 )
                    k = (k == 0) ? maxSquare - 1 : k - 1;
                mDNeighbours[(tempGrid[(i*maxSquare) + j] * 4) + 1] = tempGrid[(k*maxSquare) + j];

                //search right
                k = (j == maxSquare - 1) ? 0 : j + 1;
                while( tempGrid[(i*maxSquare) + k] == -1 )
                    k = (k == maxSquare - 1) ? 0 : k + 1;
                mDNeighbours[(tempGrid[(i*maxSquare) + j] * 4) + 2] = tempGrid[(i*maxSquare) + k];

                //search down
                k = (i == maxSquare - 1) ? 0 : i + 1;
                while( tempGrid[(k*maxSquare) + j] == -1 )
                    k = (k == maxSquare - 1) ? 0 : k + 1;
                mDNeighbours[(tempGrid[(i*maxSquare) + j] * 4) + 3] = tempGrid[(k*maxSquare) + j];
            }
        }
    }
    delete[] tempGrid;
}

void NewDev1Population::evaluatePopulation()
{   //TODO: Decide what to do with this, might not use it at all?
//    int maxIt;

//    for(int i = 0 ; i < mPointPoolSize ; ++i)
//        mOptFunc->evaluate(&(mPointPoolPositions[i * mDim]), mPointPoolValues[i]);

//    for(int i = 0 ; i < mSize ; ++i)
//    {
//        if(mValues[i] < mPrevBestValues[i])
//        {
//            if(mValues[i] < mPrevBestValues[mBestPointIndex])
//                mBestPointIndex = i;
//            mPrevBestValues[i] = mValues[i];
//            maxIt = i * mDim + mDim;
//            for(int j = i * mDim ; j < maxIt ; ++j)
//                mPrevBestPositions[j] = mPositions[j];
//        }
//    }
}

void NewDev1Population::updatePopulation()
{

    double minVal = mDValues[mBestPointIndex];
    double maxVal = minVal;
    for(int i = 0 ; i < mDSize ; ++i )
        if(mDValues[i] > maxVal)
            maxVal = mDValues[i];

    double vScale;

    vScale = (double)1/(maxVal - minVal);
    if(std::isinf(vScale))
    {//nan check
        vScale = 1;
    }

    int* poolIndices = new int[mDSize + 1]; //used to assign each DParticle a number of Points
    double* tempVals = new double[mDSize]; //used to determine num particles to assign

    int minimumPoolSize = 1;    //ensure each DParticle is assigned at least one point
    int poolSize = mSize - (minimumPoolSize * mDSize);  //number of available points to distribute

    double sumVal = 0;
    for(int i = 0 ; i < mDSize ; ++i)
    {
        tempVals[i] = (double)1 + abs(randGauss()) - ((mDValues[i] - minVal) * vScale);
        sumVal += tempVals[i];
    }


    vScale =  (double)poolSize / sumVal;


    poolIndices[0] = 0;
    for(int i = 0 ; i < mDSize ; ++i)
    {
        poolIndices[i + 1] = poolIndices[i] + minimumPoolSize + (int)(tempVals[i] * vScale); //assign points to each dparticle
    }
    poolIndices[mDSize] = mSize;
    double* tempPm = new double[mDim];
    double tempR;
    for(int i = 0 ; i < mDSize ; ++i) //Generate points
    {
        sumVal = 0; //sum of mDVals for weighting
        for(int j = 0 ; j < mDim ; ++j)
        {
            sumVal = mDValues[(mDNeighbours[ (i * 4) ])]
                    + mDValues[(mDNeighbours[ (i * 4) + 1])]
                    + mDValues[(mDNeighbours[ (i * 4) + 2])]
                    + mDValues[(mDNeighbours[ (i * 4) + 3])];
            tempPm[j] = 0;
            tempPm[j] += mDPositions[ ((mDNeighbours[ (i * 4) ]) * mDim) + j ] * (mDValues[(mDNeighbours[ (i * 4) ])] / sumVal);
                    //* ((double)rand()/(double)RAND_MAX) * (mMaxPhi / 4);
            tempPm[j] += mDPositions[ ((mDNeighbours[ (i * 4) + 1 ]) * mDim) + j ] * (mDValues[(mDNeighbours[ (i * 4) + 1])] / sumVal);
                    //* ((double)rand()/(double)RAND_MAX) * (mMaxPhi / 4);
            tempPm[j] += mDPositions[ ((mDNeighbours[ (i * 4) + 2 ]) * mDim) + j ] * (mDValues[(mDNeighbours[ (i * 4) + 2])] / sumVal);
                    //* ((double)rand()/(double)RAND_MAX) * (mMaxPhi / 4);
            tempPm[j] += mDPositions[ ((mDNeighbours[ (i * 4) + 3 ]) * mDim) + j ] * (mDValues[(mDNeighbours[ (i * 4) + 3])] / sumVal);
                    //* ((double)rand()/(double)RAND_MAX) * (mMaxPhi / 4);

            if(    (   tempPm[j] < mDPositions[ ((mDNeighbours[ (i * 4) ]) * mDim) + j ]
                    && tempPm[j] < mDPositions[ ((mDNeighbours[ (i * 4) + 1]) * mDim) + j ]
                    && tempPm[j] < mDPositions[ ((mDNeighbours[ (i * 4) + 2]) * mDim) + j ]
                    && tempPm[j] < mDPositions[ ((mDNeighbours[ (i * 4) + 3]) * mDim) + j ])
                    || (tempPm[j] > mDPositions[ ((mDNeighbours[ (i * 4)]) * mDim) + j ]
                    && tempPm[j] > mDPositions[ ((mDNeighbours[ (i * 4) + 1]) * mDim) + j ]
                    && tempPm[j] > mDPositions[ ((mDNeighbours[ (i * 4) + 2]) * mDim) + j ]
                    && tempPm[j] > mDPositions[ ((mDNeighbours[ (i * 4) + 3]) * mDim) + j ]))
            {
                tempPm[j] = 0;//debug
            }

            for( int k = poolIndices[i] ; k < poolIndices[i + 1] ; ++k)
            {
                mPositions[(k * mDim) + j] = tempPm[j] + (randGauss() * ( (mDPositions[(i*mDim) + j]) - tempPm[j] + 0.001 ) );
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

    vScale = maxVal -  minVal;
    int bestInd = -1;
    for(int i = 0 ; i < mDSize ; ++i) //update points
    {                                               //minVal and maxVal still valid from before
        tempR = ((double)rand()/(double)RAND_MAX) * ( mDValues[i] - minVal ) / ( vScale );

        if(tempR < 0.2)//select best value (including own) TODO:fix?
        {
            for( int k = poolIndices[i] ; k < poolIndices[i + 1] ; ++k)
            {
                if(mValues[k] < mDValues[i])
                {
                    if(mValues[k] < mDValues[mBestPointIndex])
                        mBestPointIndex = i;
                    bestInd = k;
                    mDValues[i] = mValues[k];
                }
            }

        }
        else//select random value
        {
            bestInd = (int)(((double)rand()/(double)RAND_MAX) * (poolIndices[i+1] - poolIndices[i])) + poolIndices[i];//random position
            mDValues[i] = mValues[bestInd];
            if(mDValues[i] < mDValues[mBestPointIndex])
                mBestPointIndex = bestInd;
        }
        if(bestInd != -1)
        {
            for(int j = 0 ; j < mDim ; ++j)
            {
                mDPositions[(i*mDim) + j] = mPositions[(bestInd * mDim) + j];
            }
        }
    }

    delete[] tempPm;
    delete[] poolIndices;
    delete[] tempVals;

}
