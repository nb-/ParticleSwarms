#include "newdev3controller.h"
#include "newdev3population.h"

NewDev3Controller::NewDev3Controller()
    :NewDev2Controller()
{
}

void NewDev3Controller::initializeOptimizer(OptimizationFunction *optFunc, double *initRange)
{
    OptimizerController::initializeOptimizer(optFunc, initRange);

    if(mDPopSpin == 0) return;
    if(optFunc == 0) return;
    if(mPopulation != 0)
        delete mPopulation;

    int dPop = mDPopSpin->value();
    int pPop = mPPopSpin->value();

    mPopulation = new NewDev3Population(pPop,dPop,optFunc);
    mPopulation->initializePopulation(initRange);

    mBestVals.push_back(mPopulation->getBestValueFound());
}
