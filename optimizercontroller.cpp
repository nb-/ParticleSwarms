#include "optimizercontroller.h"


OptimizerController::OptimizerController()
   :mPopulation(0),
    mParLayout(0),
    mDataGraph(0),
    mBestPointGraph(0),
    mOptimalPointGraph(0),
    mValueGraph(0),
    mBestVals()
{

}

OptimizerController::~OptimizerController()
{

    mBestVals.clear();
    delete mPopulation;

    foreach (QObject *object, mParLayout->parent()->children()) {//hack to get it to work, fix maybe
      QWidget *widget = qobject_cast<QWidget*>(object);
      if (widget) {
        delete widget;
      }
    }

    delete mParLayout;
    delete mDataGraph;
    delete mOptimalPointGraph;
    delete mBestPointGraph;
}

void OptimizerController::plotData(int xDim, int yDim) const
{
    if(mDataGraph == 0 || mBestPointGraph == 0) return;

    mDataGraph->clearData();
    mOptimalPointGraph->clearData();
    mBestPointGraph->clearData();

    double* dataPointer;

    for(int i = 0; i < mPopulation->getPopSize(); ++i)
    {
        dataPointer = mPopulation->getPositionPointer(i);
        mDataGraph->addData( dataPointer[xDim], dataPointer[yDim] );
    }

    dataPointer = mPopulation->getOptimizationFunction()->getOptimalPoint();
    mOptimalPointGraph->addData(dataPointer[xDim], dataPointer[yDim]);

    dataPointer = mPopulation->getBestPositionFoundPointer();
    mBestPointGraph->addData( dataPointer[xDim], dataPointer[yDim] );
}

void OptimizerController::graphValues() const
{

    mValueGraph->clearData();
    for(int i = 0 ; i < mBestVals.size() ; ++i)
    {
        mValueGraph->addData(i, mBestVals[i]);
    }
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
    mBestPointGraph->setPen( QPen( QBrush(QColor("red")), 2 ));

    if(mOptimalPointGraph==0) mOptimalPointGraph = plot->addGraph();
    mOptimalPointGraph->clearData();
    mOptimalPointGraph->setLineStyle( QCPGraph::lsNone );
    mOptimalPointGraph->setScatterStyle( QCP::ssPlusCircle );
    mOptimalPointGraph->setScatterSize(10);
    mOptimalPointGraph->setPen( QPen( QBrush(QColor("green")), 2 ));

    if(mValueGraph==0) mValueGraph = graph->addGraph();
    mValueGraph->clearData();
    mValueGraph->setLineStyle( QCPGraph::lsLine );
    mValueGraph->setScatterStyle( QCP::ssNone );
    mValueGraph->setScatterSize(4);
    mValueGraph->setPen( QPen( QBrush(QColor("red")), 1 ));
}

void OptimizerController::removeGraphs(QCustomPlot *plot, QCustomPlot* graph)
{
    if(mDataGraph!=0)
        plot->removeGraph(mDataGraph);//remove should free data?
    if(mBestPointGraph!=0)
        plot->removeGraph(mBestPointGraph);
    if(mOptimalPointGraph!=0)
        plot->removeGraph(mOptimalPointGraph);
    if(mValueGraph!=0)
        graph->removeGraph(mValueGraph);
    mOptimalPointGraph = 0;
    mDataGraph = 0;
    mBestPointGraph = 0;
    mValueGraph = 0;
}

void OptimizerController::step()
{
    mPopulation->step();
    mBestVals.push_back(mPopulation->getBestValueFound());
}
void OptimizerController::runFor(int iterations, bool graph)
{
    if(graph)
    {
        while(iterations > 0)
        {
            mPopulation->step();
            mBestVals.push_back(mPopulation->getBestValueFound());
            --iterations;
        }
    }
    else
    {
        while(iterations > 0)
        {
            mPopulation->step();
            mBestVals.push_back(mPopulation->getBestValueFound());
            --iterations;
        }
    }
}

double OptimizerController::getBestValue() const { return mPopulation->getBestValueFound(); }
int OptimizerController::getGenNum() const { return mPopulation->getGenerationNumber(); }
int OptimizerController::getPopulationSize() const { return mPopulation->getPopSize(); }
int OptimizerController::getDimension() const { return mPopulation->getDim(); }

std::vector<double> OptimizerController::getBestVals()
{
    return mBestVals;
}
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

void OptimizerController::initializeOptimizer(OptimizationFunction *optFunc, double *initRange)
{
    mBestVals.clear();
}


