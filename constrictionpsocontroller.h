#ifndef CONSTRICTIONPSOCONTROLLER_H
#define CONSTRICTIONPSOCONTROLLER_H

#include "canonpsocontroller.h"

class ConstrictionPSOController : public CanonPSOController
{
public:
    ConstrictionPSOController();
    virtual ~ConstrictionPSOController();

    virtual void setParameterBox(QWidget* parent);
    virtual void initializeOptimizer(OptimizationFunction* optFunc);
};

#endif // CONSTRICTIONPSOCONTROLLER_H
