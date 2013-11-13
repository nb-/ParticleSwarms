#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <math.h>

inline double randGauss()
{//gaussian distribution with mean 0 and std 1
    double result = -6;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    result += (double)rand() / RAND_MAX;
    return result;
}

inline double randDoubleExp()
{//double-exponential distribution with mean 0 and std 1
    return log(rand()) - log(rand());
}


#endif // MISC_H
