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
    QDoubleSpinBox* mPAccSpin;
    QDoubleSpinBox* mGAccSpin;

public:
    BareBonesPSOController();
    virtual ~BareBonesPSOController();

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc);
};

#endif // BAREBONESPSOCONTROLLER_H
