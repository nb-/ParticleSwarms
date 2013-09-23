#ifndef PSODISTTHING_H
#define PSODISTTHING_H

#include <Eigen/Core>
#include "point.h"
#include "distributionagent.h"

class PSODistThing
{
private:
    double mGlobalBestVal;
    Eigen::MatrixXd mGlobalBestPoint;

    double mCurrentBest;
    Eigen::MatrixXd mCurrentBestPoint;

    Point** mPoints;
    int mNumPoints;

    DistributionAgent* mDistAgents;
    int mNumDistAgents;

    void gaussIRandPoint(int numberOfPoints, Point* outPoints);

    double (*mOpFunction)(Eigen::MatrixXd&);

    Eigen::MatrixXd mInitializationRange;

    int mDimension;

    bool running;

public:
    PSODistThing();
    PSODistThing( int numDistAgents, int numPoints, double (*mOpFunction)(Eigen::MatrixXd&), Eigen::MatrixXd initializationRange, bool minimize = true);

    void setNumPoints(int);
    int getNumPoints();

    Point** getPoints();

    void setNumDistAgents(int);
    int getNumDistAgents();

    DistributionAgent** getDistAgents();

    void initialize( int numDistAgents, int numPoints, double (*mOpFunction)(Eigen::MatrixXd&), Eigen::MatrixXd initializationRange, bool minimize = true);
    void start();
    void pause();
    void step();
    void stop();

    double getGlobalBestVal();
    Eigen::MatrixXd getGlobalBestPoint();

    double getCurrentBestVal();
    Eigen::MatrixXd getCurrentBestPoint();

};

#endif // PSODISTTHING_H
