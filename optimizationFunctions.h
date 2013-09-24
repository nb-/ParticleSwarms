#ifndef OPTIMIZATIONFUNCTIONS_H
#define OPTIMIZATIONFUNCTIONS_H

//==================================================================
//    Optimiziation Benchmark Functions
//==================================================================

//TODO: Possibly change from simple function pointers to objects to make shifting/rotating space easier and provide defaults

#define _USE_MATH_DEFINES
#include <math.h>

class OptimizationFunctions
{
public:
    static void Sphere(int dim, double *position, double &value)
    {
        value = 0;
        for(int i = 0 ; i < dim ; ++i)
            value += position[i] * position[i];
        value = sqrt(value);
    }

    static void Rastrigin(int dim, double *position, double &value)
    {
        double A = 10;
        double w = 1;
        value = A * dim;
        for(int i = 0 ; i < dim ; ++i)
            value += (position[i] * position[i]) - (A * cos(2*M_PI*position[i]));
    }
};

#endif // OPTIMIZATIONFUNCTIONS_H
