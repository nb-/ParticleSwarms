#ifndef CANONPSOCONTROLLER_H
#define CANONPSOCONTROLLER_H

#include "optimizercontroller.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QDoubleSpinBox>

class CanonPSOController : public OptimizerController
{
protected:

    QSpinBox* mPopSpin;
    QSpinBox* mDimSpin;
    QDoubleSpinBox* mInertiaSpin;
    QDoubleSpinBox* mPAccSpin;
    QDoubleSpinBox* mGAccSpin;

public:
    CanonPSOController();
    virtual ~CanonPSOController();

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer();
};

#endif // CANONPSOCONTROLLER_H
