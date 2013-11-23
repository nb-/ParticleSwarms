#include "testrunner.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>

TestRunner::TestRunner(OptimizerController *opt, OptimizationFunction *func, int maxIterations, int repetitions, bool saveToFile, QString fileName, double* range, QObject *parent)
    :QObject(parent)
{
    mOpt = opt;
    mFunction = func;
    mIterations = maxIterations;
    mRepetitions = repetitions;
    mSaveToFile = saveToFile;
    mInitRange = range;
    mFileName = fileName;
}

TestRunner::~TestRunner()
{

}

int TestRunner::getMaxIt()
{
    return mIterations;
}

int TestRunner::getRepetitions()
{
    return mRepetitions;
}

void TestRunner::run()
{
    QFile outFile;
    if(mSaveToFile)
    {
        outFile.setFileName(mFileName);
        mSaveToFile = outFile.open(QIODevice::WriteOnly | QIODevice::Text);
    }
    mValues.clear();
    for(int i = 0 ; i < mRepetitions ; ++i)
    {
        mOpt->initializeOptimizer(mFunction,mInitRange);
        mOpt->runFor(mIterations, false);
        mValues.push_back(mOpt->getBestVals());
        emit progressUpdate(i+1,mRepetitions, mValues[i][mIterations]);
    }

    if(mSaveToFile)
    {
        QTextStream outStream(&outFile);

        for( int i = 0 ; i < mRepetitions ; ++i)
        {
            for(int j = 0 ; j <= mIterations ; ++j)
            {
                outStream << mValues[i][j] << ",";
            }
            outStream << "\n";
        }

        outFile.close();
    }

    emit finished(this);
}
