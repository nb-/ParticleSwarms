#include "constrictionpsocontroller.h"
#include "constrictionpsopopulation.h"

#include "optimizationFunctions.h"

ConstrictionPSOController::ConstrictionPSOController()
    :CanonPSOController()
{
}

ConstrictionPSOController::~ConstrictionPSOController(){}

void ConstrictionPSOController::setParameterBox(QWidget *parent)
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

    mPAccSpin = new QDoubleSpinBox();
    mPAccSpin->setMinimum(0);
    mPAccSpin->setMaximum(99);
    mPAccSpin->setDecimals(4);
    mPAccSpin->setSingleStep(0.05);
    mPAccSpin->setValue(2.05);

    mGAccSpin = new QDoubleSpinBox();
    mGAccSpin->setMinimum(0);
    mGAccSpin->setMaximum(99);
    mGAccSpin->setDecimals(4);
    mGAccSpin->setSingleStep(0.05);
    mGAccSpin->setValue(2.05);

    QLabel* title = new QLabel("Constrction PSO");
    title->setAlignment(Qt::AlignCenter);
    mParLayout->addWidget(title,                        0,0,1,2);

    mParLayout->addWidget(new QLabel("Pop Size: "),     1,0,1,1);
    mParLayout->addWidget( mPopSpin,                    1,1,1,1);

    mParLayout->addWidget(new QLabel("PBest Accel: "),  4,0,1,1);
    mParLayout->addWidget( mPAccSpin,                   4,1,1,1);

    mParLayout->addWidget(new QLabel("GBest Accel: "),  5,0,1,1);
    mParLayout->addWidget( mGAccSpin,                   5,1,1,1);

}

void ConstrictionPSOController::initializeOptimizer(OptimizationFunction *optFunc, double *initRange)
{//todo: add null checks
    if(mPopSpin == 0) return;
    if(optFunc == 0) return;

    if(mPopulation != 0)
        delete mPopulation;

    int pop = mPopSpin->value();
    double pA = mPAccSpin->value();
    double gA = mGAccSpin->value();

    mPopulation = new ConstrictionPSOPopulation(pop,pA,gA, optFunc);
    mPopulation->initializePopulation(initRange);

    mBestVals.push_back(mPopulation->getBestValueFound());
}
