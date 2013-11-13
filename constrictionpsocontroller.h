#ifndef CONSTRICTIONPSOCONTROLLER_H
#define CONSTRICTIONPSOCONTROLLER_H

#include "canonpsocontroller.h"

class ConstrictionPSOController : public CanonPSOController
{
public:
    ConstrictionPSOController();
    virtual ~ConstrictionPSOController();

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc, double* initRange = 0);
};

#endif // CONSTRICTIONPSOCONTROLLER_H
