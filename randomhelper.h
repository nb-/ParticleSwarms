#ifndef MISC_H
#define MISC_H

#include <stdlib.h>

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

#endif // MISC_H
