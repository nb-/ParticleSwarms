#ifndef OPTIMIZERCONTROLLER_H
#define OPTIMIZERCONTROLLER_H

#include "population.h"
#include "qcustomplot.h"

class OptimizerController
{
protected:
    Population* mPopulation;

    QCPGraph* mDataGraph;
    QCPGraph* mBestPointGraph;
    QCPGraph* mValueGraph;

    int mGenNumber;

public:

    OptimizerController(Population* population);
    ~OptimizerController();

    double getBestValue();
    int getGenNum();

    void initializePopulation();
    void step();
    void runFor(int iterations);

    double getLowerBound(int dim);
    double getUpperBound(int dim);

    double getUpperFit(int dim);
    double getLowerFit(int dim);

    void plotData(int xDim, int yDim);
    void initGraphs(QCustomPlot* plot, QCustomPlot* graph);
    void removeGraphs(QCustomPlot* plot, QCustomPlot* graph);
};

#endif // OPTIMIZERCONTROLLER_H
