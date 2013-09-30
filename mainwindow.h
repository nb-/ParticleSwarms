#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "optimizationFunctions.h"
#include "optimizercontroller.h"
#include "QDebugStream.h"
#include <QDoubleSpinBox>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



private:
    Ui::MainWindow *ui;
    OptimizerController* mOptimizer;
    OptimizationFunction* mOptFunc;

    QDebugStream* qout;

    int mDim;
    QDoubleSpinBox** mUpperBoundSpins;
    QVBoxLayout* mUpperBoundLayout;

    QDoubleSpinBox** mLowerBoundSpins;
    QVBoxLayout* mLowerBoundLayout;

    QDoubleSpinBox** mScaleSpins;
    QVBoxLayout* mScaleLayout;

    QDoubleSpinBox** mTranslateSpins;
    QVBoxLayout* mTranslateLayout;

    //stuff for plotting function topologies
    bool mVisualize;
    QCPGraph** mVisualizeGraphs;
    int mVisXYRes;
    int mVisZRes;
    void initVisGraphs();
    void removeVisGraphs();

    void createOptimizer(QString optName);
    void createOptFunc();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initPlot();
    void plotData();

    void clearButtonPressed();
    void updateGraphScale();

    void visDataCheckPressed();

    void optComboChanged();
    void functionComboChanged();
    void dimSpinChanged();

    void stepButtonPressed();
    void runForButtonPressed();
    void initButtonPressed();

    void zoomBoundsButtonPressed();
    void zoomFitButtonPressed();

};


#endif // MAINWINDOW_H
