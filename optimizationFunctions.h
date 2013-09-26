#ifndef OPTIMIZATIONFUNCTIONS_H
#define OPTIMIZATIONFUNCTIONS_H

//==================================================================
//    Optimiziation Benchmark Functions
//==================================================================
//TODO: Add support for maximization functions, will require modifications to optimizer classes themselves.
//possible solution: comparator function pointers

#define _USE_MATH_DEFINES
#include <math.h>

class OptimizationFunction
{
protected:
    int mDim;
    double *mBounds;
    double *mOffsets;
    double *mStretch;
    double *mOptimalPoint;
    double mOptimalValue;
public:
    OptimizationFunction(int dim, double* bounds ,double* offsets = 0, double* stretch = 0)
    {
        mDim = dim;
        mBounds = bounds;
        mOffsets = offsets;
        mStretch = stretch;
        mOptimalPoint = 0;
        setOptimalPoint();
    }

    virtual ~OptimizationFunction()
    {
        delete[] mBounds;
        delete[] mOffsets;
        delete[] mStretch;
        delete[] mOptimalPoint;
    }

    virtual void evaluate(double* position, double &outValue) = 0;

    int getDim(){return mDim;}
    double* getOffsets(){return mOffsets;}
    double* getStretch(){return mStretch;}
    double* getOptimalPoint(){return mOptimalPoint;}
    double* getBounds(){return mBounds;}
    double getOptimalValue(){return mOptimalValue;}


protected:
    virtual void setOptimalPoint()
    {
        if(mOptimalPoint != 0) delete mOptimalPoint;

        mOptimalPoint = new double[mDim];
        for(int i = 0 ; i < mDim ; ++i)
        {
           mOptimalPoint[i] = transformInputValue(i, 0);
        }

        mOptimalValue = 0;
    }

    double transformInputValue(const int dim, double inputVal)
    {
        if(mStretch != 0)
        {
            inputVal *= mStretch[dim];
        }
        if(mOffsets != 0)
        {
            inputVal += mOffsets[dim];
        }
        return inputVal;
    }
};

class SphereFunction : public OptimizationFunction
{
public:
    SphereFunction(int dim, double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(dim, bounds, offsets, stretch) {}

    virtual void evaluate(double *position, double &outValue)
    {
        outValue = 0;
        for(int i = 0 ; i < mDim ; ++i)
            outValue += pow(transformInputValue(i, position[i]), 2);
    }
protected:
    virtual void setOptimalPoint()
    {
        if(mOptimalPoint != 0) delete mOptimalPoint;

        mOptimalPoint = new double[mDim];
        for(int i = 0 ; i < mDim ; ++i)
        {
           mOptimalPoint[i] = transformInputValue(i, 0);
        }

        mOptimalValue = 0;
    }
};

class RastriginFunction : public OptimizationFunction
{
public:
    RastriginFunction(int dim, double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(dim,bounds, offsets, stretch) {}

    virtual void evaluate(double *position, double &outValue)
    {
        outValue = 0;
        double temp = 0;
        for(int i = 0 ; i < mDim ; ++i)
        {
            temp = transformInputValue(i, position[i]);
            outValue += (temp * temp) - (10 * cos(2*M_PI*temp)) + 10;
        }
    }
protected:
    virtual void setOptimalPoint()
    {
        if(mOptimalPoint != 0) delete mOptimalPoint;

        mOptimalPoint = new double[mDim];
        for(int i = 0 ; i < mDim ; ++i)
        {
           mOptimalPoint[i] = transformInputValue(i, 0);
        }

        mOptimalValue = 0;
    }
};

#endif // OPTIMIZATIONFUNCTIONS_H
