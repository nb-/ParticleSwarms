#ifndef OPTIMIZERCONTROLLER_H
#define OPTIMIZERCONTROLLER_H

#include "population.h"
#include "qcustomplot.h"

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


public:

    OptimizerController(Population* population);
    ~OptimizerController();

    double getBestValue() const;
    int getGenNum() const;

    void initializePopulation();
    void step();
    void runFor(int iterations);

    double getLowerBound(int dim) const;
    double getUpperBound(int dim) const;

    double getUpperFit(int dim) const;
    double getLowerFit(int dim) const;

    void plotData(int xDim, int yDim) const;
    void initGraphs(QCustomPlot* plot, QCustomPlot* graph);
    void removeGraphs(QCustomPlot* plot, QCustomPlot* graph);
};

#endif // OPTIMIZERCONTROLLER_H
