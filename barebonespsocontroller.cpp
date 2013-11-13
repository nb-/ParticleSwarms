#include "barebonespsocontroller.h"
#include "barebonespsopopulation.h"
#include "optimizationFunctions.h"

BareBonesPSOController::BareBonesPSOController()
    :OptimizerController(),
      mPopSpin(0)
{
}

BareBonesPSOController::~BareBonesPSOController()
{
    delete mPopSpin;
}

void BareBonesPSOController::setParameterBox(QWidget *parent)
{
    if(mParLayout != 0)
    {
        delete mParLayout;//is this safe?
    }

    mParLayout = new QGridLayout(parent);

    mPopSpin = new QSpinBox();
    mPopSpin->setMinimum(2);
    mPopSpin->setMaximum(9999999);
    mPopSpin->setValue(10);

    QLabel* title = new QLabel("Bare Bones PSO");
    title->setAlignment(Qt::AlignCenter);
    mParLayout->addWidget(title,                        0,0,1,2);

    mParLayout->addWidget(new QLabel("Pop Size: "),     1,0,1,1);
    mParLayout->addWidget( mPopSpin,                    1,1,1,1);

}

void BareBonesPSOController::initializeOptimizer(OptimizationFunction *optFunc, double *initRange)
{//todo: add null checks
    OptimizerController::initializeOptimizer(optFunc, initRange);
    if(mPopSpin == 0) return;
    if(optFunc == 0) return;

    if(mPopulation != 0)
        delete mPopulation;

    int pop = mPopSpin->value();

    mPopulation = new BareBonesPSOPopulation(pop, optFunc);
    mPopulation->initializePopulation(initRange);
    mBestVals.push_back(mPopulation->getBestValueFound());
}
