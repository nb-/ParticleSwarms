#ifndef NEWDEV2CONTROLLER_H
#define NEWDEV2CONTROLLER_H


#include "optimizercontroller.h"
#include <QSpinBox>
#include <QLabel>
#include <QDoubleSpinBox>

class NewDev2Controller : public OptimizerController
{
protected:

    QSpinBox* mPPopSpin;
    QSpinBox* mDPopSpin;

    QCPGraph* mPointDataGraph;
public:
    NewDev2Controller();
    virtual ~NewDev2Controller();

    virtual void plotData(int xDim, int yDim) const;

    virtual void initGraphs(QCustomPlot* plot, QCustomPlot* graph);
    virtual void removeGraphs(QCustomPlot* plot, QCustomPlot* graph);

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc);

};

#endif // NEWDEV2CONTROLLER_H
