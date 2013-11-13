#include "newdev2controller.h"
#include "newdev2population.h"

NewDev2Controller::NewDev2Controller()
   :OptimizerController(),
    mPointDataGraph(0),
    mPPopSpin(0),
    mDPopSpin(0)
{
}

NewDev2Controller::~NewDev2Controller()
{
    delete mPointDataGraph;
    delete mPPopSpin;
    delete mDPopSpin;
}

void NewDev2Controller::plotData(int xDim, int yDim) const
{
    OptimizerController::plotData(xDim,yDim);
    mPointDataGraph->clearData();

    double* dataPointer;

    for(int i = 0; i < ((NewDev2Population*)mPopulation)->getDSize() ; ++i)
    {
        dataPointer = ((NewDev2Population*)mPopulation)->getDPositionPointer(i);
        mPointDataGraph->addData( dataPointer[xDim], dataPointer[yDim] );
    }

}

void NewDev2Controller::initGraphs(QCustomPlot *plot, QCustomPlot *graph)
{
    OptimizerController::initGraphs(plot,graph);

    if(mPointDataGraph==0) mPointDataGraph = plot->addGraph();

    mPointDataGraph->clearData();
    mPointDataGraph->setLineStyle( QCPGraph::lsNone );
    mPointDataGraph->setScatterStyle( QCP::ssCircle );
    mPointDataGraph->setScatterSize(4);
    mPointDataGraph->setPen( QPen( QBrush(QColor("purple")), 2 ));

    mDataGraph->setScatterSize(2);
    mDataGraph->setPen( QPen( QColor("blue") ));
}

void NewDev2Controller::removeGraphs(QCustomPlot *plot, QCustomPlot *graph)
{
    OptimizerController::removeGraphs(plot,graph);
    if(mPointDataGraph!=0)
        plot->removeGraph(mPointDataGraph);
    mPointDataGraph = 0;
}

void NewDev2Controller::setParameterBox(QWidget *parent)
{
    if(mParLayout != 0)
    {
        delete mParLayout;//is this safe?
    }

    mParLayout = new QGridLayout(parent);

    mPPopSpin = new QSpinBox();
    mPPopSpin->setMinimum(2);
    mPPopSpin->setMaximum(9999999);
    mPPopSpin->setValue(100);

    mDPopSpin = new QSpinBox();
    mDPopSpin->setMinimum(2);
    mDPopSpin->setMaximum(9999999);
    mDPopSpin->setValue(20);


    QLabel* title = new QLabel("Rough First Attempt");
    title->setAlignment(Qt::AlignCenter);
    mParLayout->addWidget(title,                        0,0,1,2);

    mParLayout->addWidget(new QLabel("Pop Size: "),     1,0,1,1);
    mParLayout->addWidget( mDPopSpin,                    1,1,1,1);

    mParLayout->addWidget(new QLabel("Point Pool Size: "),      2,0,1,1);
    mParLayout->addWidget( mPPopSpin,                   2,1,1,1);

}

void NewDev2Controller::initializeOptimizer(OptimizationFunction *optFunc, double *initRange)
{
    OptimizerController::initializeOptimizer(optFunc, initRange);

    if(mDPopSpin == 0) return;
    if(optFunc == 0) return;
    if(mPopulation != 0)
        delete mPopulation;

    int dPop = mDPopSpin->value();
    int pPop = mPPopSpin->value();

    mPopulation = new NewDev2Population(pPop,dPop,optFunc);
    mPopulation->initializePopulation(initRange);

    mBestVals.push_back(mPopulation->getBestValueFound());
}
