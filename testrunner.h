#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>
#include "optimizercontroller.h"
#include "optimizationFunctions.h"
#include "vector"

class TestRunner : public QObject
{
    Q_OBJECT
private:
    OptimizerController* mOpt;
    OptimizationFunction* mFunction;
    int mIterations;
    int mRepetitions;

    double* mInitRange;
    bool mSaveToFile;

    QString mFileName;

public:
    explicit TestRunner(OptimizerController* opt, OptimizationFunction* func, int maxIterations, int repetitions, bool saveToFile, QString fileName, double* range = 0, QObject *parent = 0);
    ~TestRunner();
    std::vector< std::vector<double> > mValues;

    int getMaxIt();
    int getRepetitions();

public slots:
    void run();
signals:
    void progressUpdate(int, int, double);
    void finished(TestRunner*);
};

#endif // TESTRUNNER_H
