#include "psodistthing.h"

PSODistThing::PSODistThing()
{
    running = false;
}
PSODistThing::PSODistThing( int numDistAgents, int numPoints, double (*mOpFunction)(Eigen::MatrixXd&), Eigen::MatrixXd initializationRange, bool minimize = true)
{
    initialize(numDistAgents, numPoints, mOpFunction, initializationRange, minimize);
    running = false;
}

void PSODistThing::gaussIRandPoint(int numberOfPoints, Point* outPoints)
{
    int i, n;
    double gaussRand;

    Eigen::MatrixXd temp(mDimension, 1);

    for( n = 0 ; n < numberOfPoints ; ++n ){
        for( i = 0 ; i < mDimension ; ++i ){

            gaussRand = -6;

            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            gaussRand += (double)rand() / RAND_MAX;
            temp(i,0) = gaussRand;

        }
        outPoints[n].mPosition = temp;
    }
}


void PSODistThing::initialize( int numDistAgents, int numPoints, double (*OpFunction)(Eigen::MatrixXd&), int dim, Eigen::MatrixXd initializationRange, bool minimize = true)
{
    if(initializationRange.rows() != dim || initializationRange.cols() != 2)
        return;

    mDimension = dim;

    mNumDistAgents = numDistAgents;
    if(mDistAgents != NULL)
        delete[] mDistAgents;

    Eigen::MatrixXd* initEig = new Eigen::MatrixXd(mDimension,mDimension);
    for(int i = 0 ; i < mDimension; ++i)
    {
        for(int j = 0 ; j < mDimension; ++j)
        {
            if(i!=j)
                initEig(i,j)=0;
            else
                initEig(i,j)=1;
        }
    }

    Eigen::MatrixXd tempMean(mDimension,1);
    double tempRand;

    mDistAgents = new DistributionAgent[mNumDistAgents];
    for(int i = 0 ; i < mNumDistAgents; ++i){

        for(int j = 0 ; j < mDimension; ++j)
        {
            tempRand = (double)rand() / RAND_MAX;
            tempMean(j,0) = (tempRand * initializationRange(j,0)) + ( ((double)1/tempRand) * initializationRange(j,1) );
        }

        mDistAgents[i] = new DistributionAgent(tempMean,initEig,1);
    }

    mNumPoints = numPoints;
    if(mNumPoints != NULL)
        delete[] mPoints;

    mPoints = new Point[mNumPoints];
    for(int i = 0 ; i < mNumPoints; ++i)
    {
        mPoints[i] = new Point();
    }
    gaussIRandPoint(mNumberOfPoints, mPoints);

    mOpFunction = OpFunction;
}

void PSODistThing::start()
{

}

void PSODistThing::pause()
{


}

void PSODistThing::step()
{



}

void PSODistThing::stop()
{

}

double PSODistThing::getGlobalBestVal()
{

}

Eigen::MatrixXd PSODistThing::getGlobalBestPoint()
{

}

double PSODistThing::getCurrentBestVal()
{

}

Eigen::MatrixXd PSODistThing::getCurrentBestPoint()
{

}
