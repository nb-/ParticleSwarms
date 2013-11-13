#ifndef BAREBONESPSOCONTROLLER_H
#define BAREBONESPSOCONTROLLER_H

#include "optimizercontroller.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDoubleSpinBox>

class BareBonesPSOController : public OptimizerController
{
protected:

    QSpinBox* mPopSpin;
public:
    BareBonesPSOController();
    virtual ~BareBonesPSOController();

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc, double* initRange = 0);
};

#endif // BAREBONESPSOCONTROLLER_H
