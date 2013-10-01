#include "barebonespsocontroller.h"
#include "barebonespsopopulation.h"
#include "optimizationFunctions.h"

BareBonesPSOController::BareBonesPSOController()
    :OptimizerController(),
      mPopSpin(0),
      mPAccSpin(0),
      mGAccSpin(0)
{
}

BareBonesPSOController::~BareBonesPSOController()
{
    delete mPopSpin;
    delete mPAccSpin;
    delete mGAccSpin;
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

    mPAccSpin = new QDoubleSpinBox();
    mPAccSpin->setMinimum(0);
    mPAccSpin->setMaximum(99);
    mPAccSpin->setSingleStep(0.1);
    mPAccSpin->setDecimals(4);
    mPAccSpin->setValue(1);

    mGAccSpin = new QDoubleSpinBox();
    mGAccSpin->setMinimum(0);
    mGAccSpin->setMaximum(99);
    mGAccSpin->setSingleStep(0.1);
    mGAccSpin->setDecimals(4);
    mGAccSpin->setValue(1);

    QLabel* title = new QLabel("Canonical PSO");
    title->setAlignment(Qt::AlignCenter);
    mParLayout->addWidget(title,                        0,0,1,2);

    mParLayout->addWidget(new QLabel("Pop Size: "),     1,0,1,1);
    mParLayout->addWidget( mPopSpin,                    1,1,1,1);

    mParLayout->addWidget(new QLabel("PBest Accel: "),  2,0,1,1);
    mParLayout->addWidget( mPAccSpin,                   2,1,1,1);

    mParLayout->addWidget(new QLabel("GBest Accel: "),  3,0,1,1);
    mParLayout->addWidget( mGAccSpin,                   3,1,1,1);

}

void BareBonesPSOController::initializeOptimizer(OptimizationFunction *optFunc)
{//todo: add null checks
    if(mPopSpin == 0) return;
    if(optFunc == 0) return;

    if(mPopulation != 0)
        delete mPopulation;

    int pop = mPopSpin->value();
    double pA = mPAccSpin->value();
    double gA = mGAccSpin->value();

    mPopulation = new BareBonesPSOPopulation(pop, pA, gA, optFunc);
    mPopulation->initializePopulation();
}
