#include "optimizercontroller.h"

OptimizerController::OptimizerController(Population* population)
   :mPopulation(population),
    mDataGraph(0),
    mBestPointGraph(0),
    mValueGraph(0),
    mGenNumber(0)
{
    population->initializePopulation();
}

OptimizerController::~OptimizerController()
{
    if(mDataGraph!=0) delete mDataGraph;
    if(mBestPointGraph!=0) delete mBestPointGraph;
    if(mPopulation!=0) delete mPopulation;
}

void OptimizerController::plotData(int xDim, int yDim)
{
    if(mDataGraph == 0 || mBestPointGraph == 0) return;

    mDataGraph->clearData();
    mBestPointGraph->clearData();

    double* dataPointer;

    for(int i = 0; i < mPopulation->getPopSize(); ++i)
    {
        dataPointer = mPopulation->getPositionPointer(i);
        mDataGraph->addData( dataPointer[xDim], dataPointer[yDim] );
    }

    dataPointer = mPopulation->getBestPositionFoundPointer();
    mBestPointGraph->addData( dataPointer[xDim], dataPointer[yDim] );

    mValueGraph->addData(mGenNumber, mPopulation->getBestValueFound());
}
void OptimizerController::initGraphs(QCustomPlot* plot, QCustomPlot* graph)
{
    if(mDataGraph==0) mDataGraph = plot->addGraph();

    mDataGraph->clearData();
    mDataGraph->setLineStyle( QCPGraph::lsNone );
    mDataGraph->setScatterStyle( QCP::ssCircle );
    mDataGraph->setScatterSize(2);
    mDataGraph->setPen( QPen( QColor("blue") ));

    if(mBestPointGraph==0) mBestPointGraph = plot->addGraph();
    mBestPointGraph->clearData();
    mBestPointGraph->setLineStyle( QCPGraph::lsNone );
    mBestPointGraph->setScatterStyle( QCP::ssCross );
    mBestPointGraph->setScatterSize(7);
    mBestPointGraph->setPen( QPen( QColor("red") ));

    if(mValueGraph==0) mValueGraph = graph->addGraph();
    mValueGraph->clearData();
    mValueGraph->setLineStyle( QCPGraph::lsLine );
    mValueGraph->setScatterStyle( QCP::ssCircle );
    mValueGraph->setScatterSize(4);
    mValueGraph->setPen( QPen( QColor("red") ));
}
void OptimizerController::removeGraphs(QCustomPlot *plot, QCustomPlot* graph)
{
    plot->removeGraph(mDataGraph);
    plot->removeGraph(mBestPointGraph);
    graph->removeGraph(mValueGraph);
    mDataGraph = 0;
    mBestPointGraph = 0;
    mValueGraph = 0;
}

void OptimizerController::initializePopulation()
{
    mPopulation->initializePopulation();
    mGenNumber = 0;
}

void OptimizerController::step()
{
    mPopulation->updatePopulation();
    ++mGenNumber;
}
void OptimizerController::runFor(int iterations)
{
    mGenNumber += iterations;
    while(iterations > 0)
    {
        mPopulation->updatePopulation();
        --iterations;
    }
}

double OptimizerController::getBestValue()
{
    return mPopulation->getBestValueFound();
}
int OptimizerController::getGenNum(){return mGenNumber;}

double OptimizerController::getLowerBound(int dim){ return mPopulation->getLowerBound(dim); }
double OptimizerController::getUpperBound(int dim){ return mPopulation->getUpperBound(dim); }

double OptimizerController::getUpperFit(int dim)
{
    if(dim >= mPopulation->getDim()) return 0;

    double* dataPointer = mPopulation->getPositionPointer(0);
    double upper = dataPointer[dim];

    for(int i = 1; i < mPopulation->getPopSize(); ++i)
    {
        dataPointer = mPopulation->getPositionPointer(i);
        if(dataPointer[dim] > upper)
            upper = dataPointer[dim];
    }
    return upper;
}

double OptimizerController::getLowerFit(int dim)
{
    if(dim >= mPopulation->getDim()) return 0;

    double* dataPointer = mPopulation->getPositionPointer(0);
    double lower = dataPointer[dim];

    for(int i = 1; i < mPopulation->getPopSize(); ++i)
    {
        dataPointer = mPopulation->getPositionPointer(i);
        if(dataPointer[dim] < lower)
            lower = dataPointer[dim];
    }
    return lower;
}

