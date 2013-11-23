#ifndef NEWDEV3CONTROLLER_H
#define NEWDEV3CONTROLLER_H

#include "newdev2controller.h"

class NewDev3Controller : public NewDev2Controller
{
public:
    NewDev3Controller();

    virtual void initializeOptimizer(OptimizationFunction* optFunc, double* initRange = 0);
};

#endif // NEWDEV3CONTROLLER_H
