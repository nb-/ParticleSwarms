#ifndef OPTIMIZERCONTROLLER_H
#define OPTIMIZERCONTROLLER_H

#include "population.h"
#include "qcustomplot.h"
#include <QGridLayout>


//==================================================================
//    Class which provides UI control interface for optimizers
//==================================================================

class OptimizerController
{
protected:
    Population* mPopulation;

    QCPGraph* mDataGraph;
    QCPGraph* mBestPointGraph;
    QCPGraph* mValueGraph;

    QGridLayout* mParLayout;

public:
    OptimizerController();
    virtual ~OptimizerController();

    double getBestValue() const;
    int getGenNum() const;

    int getPopulationSize() const;
    int getDimension() const;

    void initializePopulation();
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
    virtual void initializeOptimizer() = 0;
};

#endif // OPTIMIZERCONTROLLER_H
