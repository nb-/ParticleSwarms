#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>
#include <hash_map>

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

    ui->optimizerCombo->addItem("Canonical PSO", "canon");
    ui->optimizerCombo->addItem("Constriction Coefficient PSO", "constriction");

    QObject::connect(ui->stepButton, &QAbstractButton::clicked, this, &MainWindow::stepButtonPressed);
    QObject::connect(ui->initButton, &QAbstractButton::clicked, this, &MainWindow::initButtonPressed);
    QObject::connect(ui->zoomBoundsButton, &QAbstractButton::clicked, this, &MainWindow::zoomBoundsButtonPressed);
    QObject::connect(ui->zoomFitButton, &QAbstractButton::clicked, this, &MainWindow::zoomFitButtonPressed);

    QObject::connect(ui->runForButton, &QAbstractButton::clicked, this, &MainWindow::runForButtonPressed);

    QObject::connect(ui->plotXDimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::plotData);
    QObject::connect(ui->plotYDimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::plotData);

    qout = new QDebugStream(std::cout, ui->textEdit);

}

OptimizerController *MainWindow::createOptimizer(QString optName)
{//maybe figure out a better method of doing this


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

    ui->plot->xAxis->setRange( mOptimizer->getLowerBound( ui->plotXDimSpin->value() ), mOptimizer->getUpperBound( ui->plotXDimSpin->value() ) );
    ui->plot->yAxis->setRange( mOptimizer->getLowerBound( ui->plotYDimSpin->value() ), mOptimizer->getUpperBound( ui->plotYDimSpin->value() ) );

    ui->plot->replot();
    ui->graph->replot();
}
void MainWindow::plotData()
{
    if(mOptimizer==0) return;

    int xDim = ui->plotXDimSpin->value();
    int yDim = ui->plotYDimSpin->value();

    mOptimizer->plotData(xDim, yDim);
    ui->graph->xAxis->setRange(0, mOptimizer->getGenNum() + 1);
    ui->plot->replot();
    ui->graph->replot();
}

void MainWindow::stepButtonPressed()
{
    if(mOptimizer==0) return;
    mOptimizer->step();
    plotData();

    std::cout << "Best: " <<  mOptimizer->getBestValue() << std::endl;
}

void MainWindow::runForButtonPressed()
{
    if(mOptimizer == 0) return;
    mOptimizer->runFor( ui->runForSpin->value() );
    plotData();

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

        ui->plotXDimSpin->setMaximum(dim - 1);
        ui->plotYDimSpin->setMaximum(dim - 1);

        int popSize = ui->popSizeSpin->value();
        if(popSize < 1) popSize = 1;
        double* bounds = new double[dim * 2];
        for(int i = 0 ; i < dim; ++i)
        {
            bounds[i] = -10;
            bounds[i + dim] = 10;
        }
        Population* p = new CanonPSOPopulation(popSize, dim, 0.7, 1, 1, &(OptimizationFunctions::Sphere), bounds);
        mOptimizer = new OptimizerController(p);
        initPlot();
        plotData();
}

void MainWindow::zoomBoundsButtonPressed()
{
    ui->plot->xAxis->setRange( mOptimizer->getLowerBound(ui->plotXDimSpin->value()), mOptimizer->getUpperBound(ui->plotXDimSpin->value()) );
    ui->plot->yAxis->setRange( mOptimizer->getLowerBound(ui->plotYDimSpin->value()), mOptimizer->getUpperBound(ui->plotYDimSpin->value()) );

    ui->plot->replot();
}

void MainWindow::zoomFitButtonPressed()
{
    double xL = mOptimizer->getLowerFit(ui->plotXDimSpin->value());
    double xU = mOptimizer->getUpperFit(ui->plotXDimSpin->value());
    double padding = (xU - xL) * 0.1;
    ui->plot->xAxis->setRange( xL - padding , xU + padding);

    double yL = mOptimizer->getLowerFit(ui->plotYDimSpin->value());
    double yU = mOptimizer->getUpperFit(ui->plotYDimSpin->value());
    padding = (yU - yL) * 0.1;

    ui->plot->yAxis->setRange( yL - padding, yU + padding );

    ui->plot->replot();
}

MainWindow::~MainWindow()
{
    if(mOptimizer != 0) mOptimizer->removeGraphs(ui->plot, ui->graph);
    delete mOptimizer;
    delete qout;
    delete ui;
}


