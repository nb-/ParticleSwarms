#ifndef NEWDEV1CONTROLLER_H
#define NEWDEV1CONTROLLER_H

#include "optimizercontroller.h"
#include <QSpinBox>
#include <QLabel>
#include <QDoubleSpinBox>

class NewDev1Controller : public OptimizerController
{
protected:

    QSpinBox* mPPopSpin;
    QSpinBox* mDPopSpin;
    QDoubleSpinBox* mMaxPhiSpin;

    QCPGraph* mPointDataGraph;
public:
    NewDev1Controller();
    virtual ~NewDev1Controller();

    virtual void plotData(int xDim, int yDim) const;

    virtual void initGraphs(QCustomPlot* plot, QCustomPlot* graph);
    virtual void removeGraphs(QCustomPlot* plot, QCustomPlot* graph);

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc);

};

#endif // NEWDEV1CONTROLLER_H
