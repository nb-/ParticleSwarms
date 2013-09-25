#include "optimizercontroller.h"


OptimizerController::OptimizerController()
   :mPopulation(0),
    mParLayout(0),
    mDataGraph(0),
    mBestPointGraph(0),
    mValueGraph(0)
{
}

OptimizerController::~OptimizerController()
{
    delete mPopulation;

    foreach (QObject *object, mParLayout->parent()->children()) {
      QWidget *widget = qobject_cast<QWidget*>(object);
      if (widget) {
        delete widget;
      }
    }

    delete mParLayout;
    delete mDataGraph;
    delete mBestPointGraph;
}

void OptimizerController::plotData(int xDim, int yDim) const
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
}

void OptimizerController::graphValues() const
{
    mValueGraph->addData(mPopulation->getGenerationNumber(), mPopulation->getBestValueFound());
}

void OptimizerController::initGraphs(QCustomPlot* plot, QCustomPlot* graph)
{
    if(mDataGraph==0) mDataGraph = plot->addGraph();

    mDataGraph->clearData();
    mDataGraph->setLineStyle( QCPGraph::lsNone );
    mDataGraph->setScatterStyle( QCP::ssCircle );
    mDataGraph->setScatterSize(3);
    mDataGraph->setPen( QPen( QColor("blue") ));

    if(mBestPointGraph==0) mBestPointGraph = plot->addGraph();
    mBestPointGraph->clearData();
    mBestPointGraph->setLineStyle( QCPGraph::lsNone );
    mBestPointGraph->setScatterStyle( QCP::ssCross );
    mBestPointGraph->setScatterSize(7);
    mBestPointGraph->setPen( QPen( QBrush(QColor("red")), 3 ));

    if(mValueGraph==0) mValueGraph = graph->addGraph();
    mValueGraph->clearData();
    mValueGraph->setLineStyle( QCPGraph::lsLine );
    mValueGraph->setScatterStyle( QCP::ssNone );
    mValueGraph->setScatterSize(4);
    mValueGraph->setPen( QPen( QBrush(QColor("red")), 1 ));
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
}

void OptimizerController::step()
{
    mPopulation->step();
}
void OptimizerController::runFor(int iterations, bool graph)
{
    if(graph)
    {
        while(iterations > 0)
        {
            mPopulation->step();
            graphValues();
            --iterations;
        }
    }
    else
    {
        while(iterations > 0)
        {
            mPopulation->step();
            --iterations;
        }
    }
}

double OptimizerController::getBestValue() const { return mPopulation->getBestValueFound(); }
int OptimizerController::getGenNum() const { return mPopulation->getGenerationNumber(); }
int OptimizerController::getPopulationSize() const { return mPopulation->getPopSize(); }
int OptimizerController::getDimension() const { return mPopulation->getDim(); }
double OptimizerController::getLowerBound(int dim) const{ return mPopulation->getLowerBound(dim); }
double OptimizerController::getUpperBound(int dim) const{ return mPopulation->getUpperBound(dim); }

double OptimizerController::getUpperFit(int dim) const
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

double OptimizerController::getLowerFit(int dim) const
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

void OptimizerController::removeParameterBox(QWidget *parent)
{
    delete mParLayout;
}


