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
        mOptimalPoint = 0;//must set during first call to getOptPoint to invoke child overrides
        mOptimalValue = 0;//set in construtors, no modification of space should effect this
    }

    virtual ~OptimizationFunction()
    {
        delete[] mBounds;
        delete[] mOffsets;
        delete[] mStretch;
        delete[] mOptimalPoint;
    }

    static double* getDefaultBounds(int dim)
    {
        double* defBounds = new double[dim * 2];
        for(int i = 0 ; i < dim ; ++i)
        {
            defBounds[i] = -100;
            defBounds[i+dim] = 100;
        }
        return defBounds;
    }

    virtual void evaluate(double* position, double &outValue) = 0;

    int getDim(){return mDim;}
    double* getOffsets(){return mOffsets;}
    double* getStretch(){return mStretch;}
    double* getOptimalPoint()
    {
        if(mOptimalPoint == 0)
        {
            setOptimalPoint();
        }
        return mOptimalPoint;
    }
    double* getBounds()
    {
        if(mBounds == 0)
            mBounds = getDefaultBounds(mDim);
        return mBounds;
    }
    double getOptimalValue(){ return mOptimalValue; }


protected:
    virtual void setOptimalPoint() // most test functions have optima at origin, so this is default
    {
        if(mOptimalPoint != 0) delete[] mOptimalPoint;

        mOptimalPoint = new double[mDim];
        for(int i = 0 ; i < mDim ; ++i)
        {
            mOptimalPoint[i] = 0;
        }
        transformOptimalPoint();
    }
    void transformOptimalPoint()//todo add protection from multiple calls?
    {
        for(int i = 0 ; i < mDim ; ++i)
        {
            if(mStretch!=0)
            {
                mOptimalPoint[i] *= mStretch[i];
            }
            if(mOffsets != 0)
            {
                mOptimalPoint[i] += mOffsets[i];  //optimal point must be moved WITH space rather than against it unlike points
            }
        }
    }

    double transformInputValue(const int dim, double inputVal)
    {

        if(mOffsets != 0)
        {
            inputVal -= mOffsets[dim];  // shifting space by offset means shifting points the other way
        }
        if(mStretch != 0)
        {
            inputVal /= mStretch[dim];
        }

        return inputVal;
    }
};

class SphereFunction : public OptimizationFunction
{
public:
    SphereFunction(int dim, double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(dim, bounds, offsets, stretch) {}
    //optimal value is 0, no need to change

    static double* getDefaultBounds(int dim){return OptimizationFunction::getDefaultBounds(dim);}

    virtual void evaluate(double *position, double &outValue)
    {
        outValue = 0;
        for(int i = 0 ; i < mDim ; ++i)
            outValue += pow(transformInputValue(i, position[i]), 2);
    }
};

class GeneralizedRastriginFunction : public OptimizationFunction
{
public:
    GeneralizedRastriginFunction(int dim, double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(dim,bounds, offsets, stretch) {}
    //optimal value is 0, no need to change

    static double* getDefaultBounds(int dim)
    {
        double* defBounds = new double[dim * 2];
        for(int i = 0 ; i < dim ; ++i)
        {
            defBounds[i] = -5.12;
            defBounds[i+dim] = 5.12;
        }
        return defBounds;
    }

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
};

class RosenbrockFunction : public OptimizationFunction
{   //Rosenbrock Function, restricted to 2D
public:
    RosenbrockFunction(double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(2,bounds,offsets,stretch){}

    static double* getDefaultBounds(int dim)
    {
        return new double[4] { -2.048, -2.048, 2.048, 2.048 };
    }

    virtual void evaluate(double *position, double &outValue)
    {
        double tempX = transformInputValue(0,position[0]);
        double tempY = transformInputValue(1,position[1]);
        outValue = pow(1-tempX,2) + (100*pow((tempY-(tempX * tempX)), 2));
    }

    virtual void setOptimalPoint()
    {
        if(mOptimalPoint != 0) delete[] mOptimalPoint;

        mOptimalPoint = new double[mDim];
        mOptimalPoint[0] = 1;
        mOptimalPoint[1] = 1;
        transformOptimalPoint();
    }
};

class BealesFunction : public OptimizationFunction
{
public:
    BealesFunction(double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(2,bounds,offsets,stretch){}

    static double* getDefaultBounds(int dim)
    {
        return new double[4] { -4.5, -4.5, 4.5, 4.5 };
    }

    virtual void evaluate(double *position, double &outValue)
    {
        outValue = 0;
        double tempX = transformInputValue(0,position[0]);
        double tempY = transformInputValue(1,position[1]);

        outValue += pow( 1.5 - tempX + (tempX*tempY), 2);
        outValue += pow( 2.25 - tempX + (tempX * pow( (tempY), 2) ), 2);
        outValue += pow( 2.625 - tempX + (tempX * pow( (tempY), 3) ),2);
    }

    virtual void setOptimalPoint()
    {
        if(mOptimalPoint != 0) delete[] mOptimalPoint;

        mOptimalPoint = new double[mDim];
        mOptimalPoint[0] = 3;
        mOptimalPoint[1] = 0.5;
        transformOptimalPoint();
    }
};

class GoldsteinPriceFunction : public OptimizationFunction
{
public:
    GoldsteinPriceFunction(double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(2,bounds,offsets,stretch){}

    static double* getDefaultBounds(int dim)
    {
        return new double[4] {-2, -2, 2, 2};
    }

    virtual void evaluate(double *position, double &outValue)
    {
        outValue = 0;
        double tempX = transformInputValue(0,position[0]);
        double tempY = transformInputValue(1,position[1]);
        double tempXY = tempX*tempY;

        outValue = 1 + ( pow(tempX + tempY + 1, 2) * (19 - (14*tempX) + (3*(tempX*tempX)) - (14*tempY) + (6*tempXY) + (3*(tempY*tempY))));
        outValue *= 30 + ( pow( (2*tempX)-(3*tempY),2 ) * ( 18 - (32*tempX) + (12*tempX*tempX) + (48*tempY) - (36*tempXY) + (27*tempY*tempY) ) );
    }
    virtual void setOptimalPoint()
    {
        if(mOptimalPoint != 0) delete[] mOptimalPoint;

        mOptimalPoint = new double[mDim];
        mOptimalPoint[0] = 0;
        mOptimalPoint[1] = -1;
        transformOptimalPoint();
    }

};

//class AckleysFunction : public OptimizationFunction
//{
//public:
//    AckleysFunction(double* bounds, double* offsets = 0, double* stretch = 0) : OptimizationFunction(2,bounds,offsets,stretch){}

//    static double* getDefaultBounds(int dim)
//    {
//        return new double[4] {-5, -5, 5, 5};
//    }

//    virtual void evaluate(double *position, double &outValue)
//    {
//        outValue = 0;
//        double tempX = transformInputValue(0,position[0]);
//        double tempY = transformInputValue(1,position[1]);
//        outValue =

//    }

//};
#endif // OPTIMIZATIONFUNCTIONS_H
