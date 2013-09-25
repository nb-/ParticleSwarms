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

    mDimSpin = new QSpinBox();
    mDimSpin->setMinimum(2);
    mDimSpin->setMaximum(9999999);

    mInertiaSpin = new QDoubleSpinBox();
    mInertiaSpin->setMinimum(0);
    mInertiaSpin->setMaximum(99);
    mInertiaSpin->setSingleStep(0.1);
    mInertiaSpin->setValue(0.7);

    mPAccSpin = new QDoubleSpinBox();
    mPAccSpin->setMinimum(0);
    mPAccSpin->setMaximum(99);
    mPAccSpin->setSingleStep(0.1);
    mPAccSpin->setValue(2);

    mGAccSpin = new QDoubleSpinBox();
    mGAccSpin->setMinimum(0);
    mGAccSpin->setMaximum(99);
    mGAccSpin->setSingleStep(0.1);
    mGAccSpin->setValue(2);

    QLabel* title = new QLabel("Constrction PSO");
    title->setAlignment(Qt::AlignCenter);
    mParLayout->addWidget(title,                        0,0,1,2);

    mParLayout->addWidget(new QLabel("Pop Size: "),     1,0,1,1);
    mParLayout->addWidget( mPopSpin,                    1,1,1,1);

    mParLayout->addWidget(new QLabel("Dimension: "),    2,0,1,1);
    mParLayout->addWidget( mDimSpin,                    2,1,1,1);

    mParLayout->addWidget(new QLabel("PBest Accel: "),  4,0,1,1);
    mParLayout->addWidget( mPAccSpin,                   4,1,1,1);

    mParLayout->addWidget(new QLabel("GBest Accel: "),  5,0,1,1);
    mParLayout->addWidget( mGAccSpin,                   5,1,1,1);

}

void ConstrictionPSOController::initializeOptimizer()
{//todo: add null checks
    if(mPopSpin == 0) return;

    int pop = mPopSpin->value();
    int dim = mDimSpin->value();
    double pA = mPAccSpin->value();
    double gA = mGAccSpin->value();
    double* bounds = new double[dim * 2];
    for(int i = 0 ; i < dim; ++i)
    {
        bounds[i] = -10;
        bounds[i + dim] = 10;
    }
    mPopulation = new ConstrictionPSOPopulation(pop,dim,pA,gA, &(OptimizationFunctions::Rastrigin), bounds);
    mPopulation->initializePopulation();
}
