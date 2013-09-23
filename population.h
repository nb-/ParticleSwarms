#ifndef POPULATION_H
#define POPULATION_H

class Population
{
protected:

    const int mSize;
    const int mDim;

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

    const int getPopSize();
    const int getDim();

    int     getBestIndividualIndex();
    double  getBestValueFound();
    double*  getBestPositionFoundPointer();

    double  getValue(int individualIndex);
    double* getPositionPointer(int individualIndex);
    double  getPrevBestValue(int individualIndex);
    double* getPrevBestPositionPointer(int individualIndex);

    double getLowerBound(int dim);
    double getUpperBound(int dim);

    virtual void initializePopulation(double* range = 0);
    virtual void evaluatePopulation();
    virtual void updatePopulation() = 0;
};
#endif // POPULATION_H
