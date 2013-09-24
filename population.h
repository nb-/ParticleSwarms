#ifndef POPULATION_H
#define POPULATION_H

//==================================================================
//    Base Class for Population Based BB Optimizer | Abstract
//==================================================================

//TODO: Abstract further, currently contains info only needed by PSO, should work for any pop based bb opt

class Population
{
protected:

    const int mSize;
    const int mDim;

    int mGenNum;

    int mBestPointIndex;

    double* mBounds;
    double* mValues;
    double* mPositions;
    double* mPrevBestValues;
    double* mPrevBestPositions;


public:
    void (*mOptFunction)(int dim, double *position, double &value);

    Population(int size, int dim, void (*optFunc)(int dim, double *position, double &value), double* bounds);
    virtual ~Population();

    int getPopSize() const;
    int getDim() const;
    int getGenerationNumber() const;

    int     getBestIndividualIndex() const;
    double  getBestValueFound() const;
    double* getBestPositionFoundPointer() const;

    double  getValue(int individualIndex) const;
    double* getPositionPointer(int individualIndex) const;
    double  getPrevBestValue(int individualIndex) const;
    double* getPrevBestPositionPointer(int individualIndex) const;

    double getLowerBound(int dim) const;
    double getUpperBound(int dim) const;

    void step();

    virtual void initializePopulation(double* range = 0);
    virtual void evaluatePopulation();
    virtual void updatePopulation() = 0;
};
#endif // POPULATION_H
