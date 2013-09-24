#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "optimizercontroller.h"
#include "QDebugStream.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



private:
    Ui::MainWindow *ui;
    OptimizerController* mOptimizer;

    QDebugStream* qout;

    OptimizerController* createOptimizer(QString optName);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initPlot();
    void plotData();

    void stepButtonPressed();
    void runForButtonPressed();
    void initButtonPressed();

    void zoomBoundsButtonPressed();
    void zoomFitButtonPressed();

};


#endif // MAINWINDOW_H
