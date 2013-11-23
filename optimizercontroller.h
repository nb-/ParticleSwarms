#ifndef OPTIMIZERCONTROLLER_H
#define OPTIMIZERCONTROLLER_H

#include "population.h"
#include "qcustomplot.h"
#include <QGridLayout>
#include "optimizationFunctions.h"


//==================================================================
//    Class which provides UI control interface for optimizers
//==================================================================

#include <vector>

class OptimizerController
{
protected:
    Population* mPopulation;

    QCPGraph* mDataGraph;
    QCPGraph* mBestPointGraph;
    QCPGraph* mOptimalPointGraph;
    QCPGraph* mValueGraph;

    QGridLayout* mParLayout;

    std::vector<double> mBestVals;

public:
    OptimizerController();
    virtual ~OptimizerController();

    double getBestValue() const;
    int getGenNum() const;

    int getPopulationSize() const;
    int getDimension() const;

    std::vector<double> getBestVals();

    void initializePopulation(OptimizationFunction *optFunc);
    void step();
    void runFor(int iterations, bool graph);

    double getLowerBound(int dim) const;
    double getUpperBound(int dim) const;

    double getUpperFit(int dim) const;
    double getLowerFit(int dim) const;

    virtual void plotData(int xDim, int yDim) const;
    virtual void graphValues() const;

    virtual void initGraphs(QCustomPlot* plot, QCustomPlot* graph);
    virtual void removeGraphs(QCustomPlot* plot, QCustomPlot* graph);

    virtual void setParameterBox(QWidget* parent) = 0;
    void removeParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc, double* initRange = 0);

};

#endif // OPTIMIZERCONTROLLER_H
