#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>

#include "population.h"
#include "canonpsopopulation.h"
#include "optimizationFunctions.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mOptimizer(0)
{
    ui->setupUi(this);
    srand(QDateTime::currentMSecsSinceEpoch());

    QObject::connect(ui->stepButton, &QAbstractButton::clicked, this, &MainWindow::stepButtonPressed);
    QObject::connect(ui->initButton, &QAbstractButton::clicked, this, &MainWindow::initButtonPressed);

    qout = new QDebugStream(std::cout, ui->textEdit);

}
void MainWindow::initPlot()
{
    if(mOptimizer==0) return;
    mOptimizer->removeGraphs(ui->plot, ui->graph);
    ui->plot->clearGraphs();
    ui->graph->clearGraphs();

    mOptimizer->initGraphs(ui->plot, ui->graph);
    ui->plot->setTitle("Population");
    ui->graph->setTitle("Best Value Found");
    ui->graph->yAxis->setRange(0, mOptimizer->getBestValue());
    ui->plot->replot();
    ui->graph->replot();
}
void MainWindow::plotData(int xDim, int yDim)
{
    if(mOptimizer==0) return;

    ui->plot->xAxis->setRange( mOptimizer->getLowerBound(xDim), mOptimizer->getUpperBound(xDim) );
    ui->plot->yAxis->setRange( mOptimizer->getLowerBound(yDim), mOptimizer->getUpperBound(yDim) );

    mOptimizer->plotData(xDim, yDim);
    ui->graph->xAxis->setRange(0, mOptimizer->getGenNum() + 1);

    ui->plot->replot();
    ui->graph->replot();
}

void MainWindow::stepButtonPressed()
{
    if(mOptimizer==0) return;
    mOptimizer->step();
    plotData(0,1);

    std::cout << "Best: " <<  mOptimizer->getBestValue() << std::endl;
}
void MainWindow::initButtonPressed()
{

        if(mOptimizer!=0)
        {
            mOptimizer->removeGraphs(ui->plot, ui->graph);
            delete mOptimizer;
        }

        int dim = ui->dimNumSpin->value();
        if(dim < 1) dim = 1;
        int popSize = ui->popSizeSpin->value();
        if(popSize < 1) popSize = 1;
        double* bounds = new double[dim * 2];
        for(int i = 0 ; i < dim; ++i)
        {
            bounds[i] = -10;
            bounds[i + dim] = 10;
        }
        Population* p = new CanonPSOPopulation(popSize, dim, 0.7, 1, 1, &(OptimizationFunctions::Rastrigin), bounds);
        mOptimizer = new OptimizerController(p);
        initPlot();
}

MainWindow::~MainWindow()
{
    if(mOptimizer != 0) mOptimizer->removeGraphs(ui->plot, ui->graph);
    delete mOptimizer;
    delete qout;
    delete ui;
}


