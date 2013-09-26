#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>
#include <hash_map>

#include "population.h"
#include "canonpsopopulation.h"
#include "optimizationFunctions.h"
#include "canonpsocontroller.h"
#include "constrictionpsocontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mOptimizer(0),
    mOptFunc(0),
    mUpperBoundSpins(0),
    mUpperBoundLayout(0),
    mLowerBoundSpins(0),
    mDim(2)
{
    ui->setupUi(this);
    srand(QDateTime::currentMSecsSinceEpoch());

    ui->optimizerCombo->addItem("Canonical PSO", "canon");
    ui->optimizerCombo->addItem("Constriction Coefficient PSO", "constriction");

    ui->functionCombo->addItem("Sphere Function", "sphere");
    ui->functionCombo->addItem("Rastrigin Function", "rastrigin");

    QObject::connect(ui->stepButton, &QAbstractButton::clicked, this, &MainWindow::stepButtonPressed);
    QObject::connect(ui->initButton, &QAbstractButton::clicked, this, &MainWindow::initButtonPressed);
    QObject::connect(ui->zoomBoundsButton, &QAbstractButton::clicked, this, &MainWindow::zoomBoundsButtonPressed);
    QObject::connect(ui->zoomFitButton, &QAbstractButton::clicked, this, &MainWindow::zoomFitButtonPressed);

    QObject::connect(ui->runForButton, &QAbstractButton::clicked, this, &MainWindow::runForButtonPressed);

    QObject::connect(ui->plotXDimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::plotData);
    QObject::connect(ui->plotYDimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::plotData);

    QObject::connect(ui->logCheckBox, &QCheckBox::clicked, this, &MainWindow::updateGraphScale);

    QObject::connect(ui->optimizerCombo,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::optComboChanged);

    QObject::connect(ui->dimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::dimSpinChanged);


    qout = new QDebugStream(std::cout, ui->textEdit);  


    mUpperBoundLayout = new QVBoxLayout(ui->upperBoundsScroll);
    mLowerBoundLayout = new QVBoxLayout(ui->lowerBoundsScroll);

    optComboChanged();
    dimSpinChanged();
    createOptFunc();
}

void MainWindow::createOptimizer(QString optName)
{
    if(mOptimizer!=0)
    {
        mOptimizer->removeGraphs(ui->plot, ui->graph);

        //currently parameter box is cleared in a hackish way
        //todo: find a better way

        delete mOptimizer;
        mOptimizer = 0;
    }

    if( QString::compare( optName, "canon" ) == 0) // Canonical PSO
    {
        mOptimizer = new CanonPSOController();
    }
    else if ( QString::compare( optName, "constriction" ) == 0) // Constriction PSO
    {
        mOptimizer = new ConstrictionPSOController();
    }
    else
    {
        return;
    }

    mOptimizer->setParameterBox(ui->parameterContainer);

}

void MainWindow::createOptFunc()
{

    if(mOptFunc!=0) delete mOptFunc;    //todo make this more safe if optimizer is not guaranteed to be initializing
    mOptFunc = 0;

    QString optFuncName = ui->functionCombo->itemData(ui->functionCombo->currentIndex()).toString();

    int dim = ui->dimSpin->value();

    double* bounds = new double[dim*2];

    for(int i = 0 ; i < dim ; ++i)
    {
        bounds[i] = mLowerBoundSpins[i]->value();
        bounds[i + dim] = mUpperBoundSpins[i]->value();
    }

    if( QString::compare( optFuncName, "sphere") )
    {
        mOptFunc = new SphereFunction(dim, bounds);
    }
    else if( QString::compare( optFuncName, "rastrigin") )
    {
        mOptFunc = new RastriginFunction(dim, bounds);
    }

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

    updateGraphScale();

    ui->plot->replot();
    ui->graph->replot();
}
void MainWindow::plotData()
{
    if(mOptimizer==0) return;

    int xDim = ui->plotXDimSpin->value();
    int yDim = ui->plotYDimSpin->value();

    mOptimizer->plotData(xDim, yDim);
    mOptimizer->graphValues();
    ui->graph->xAxis->setRange(0, mOptimizer->getGenNum() + 1);
    ui->plot->replot();
    ui->graph->replot();
}

void MainWindow::updateGraphScale()
{
    if(ui->logCheckBox->checkState() == Qt::Checked)
        ui->graph->yAxis->setScaleType(QCPAxis::stLogarithmic);
    else
        ui->graph->yAxis->setScaleType(QCPAxis::stLinear);

    ui->graph->yAxis->setScaleLogBase( 10 );
    ui->graph->replot();
}

void MainWindow::optComboChanged()
{
    QString what = ui->optimizerCombo->itemData(ui->optimizerCombo->currentIndex()).toString();
    createOptimizer(what);//TODO: add protection from seqfault caused by changing combo box then not initializing afterwards
}

void MainWindow::dimSpinChanged()
{
    int dim = ui->dimSpin->value();
    if(dim < 2) return;

    if(mUpperBoundSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mUpperBoundSpins[i];

        delete[] mUpperBoundSpins;//pointers held will have been deleted already
    }

    mUpperBoundSpins = new QDoubleSpinBox*[dim];
    for(int i = 0 ; i < dim ; ++i)
    {
        mUpperBoundSpins[i] = new QDoubleSpinBox();
        mUpperBoundSpins[i]->setMaximum(9999999);
        mUpperBoundSpins[i]->setMinimum(-9999999);
        mUpperBoundSpins[i]->setValue(100);
        mUpperBoundLayout->addWidget(mUpperBoundSpins[i]);
    }

    if(mLowerBoundSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mLowerBoundSpins[i];

        delete[] mLowerBoundSpins;//pointers held will have been deleted already
    }

    mLowerBoundSpins = new QDoubleSpinBox*[dim];
    for(int i = 0 ; i < dim ; ++i)
    {
        mLowerBoundSpins[i] = new QDoubleSpinBox();
        mLowerBoundSpins[i]->setMaximum(9999999);
        mLowerBoundSpins[i]->setMinimum(-9999999);
        mLowerBoundSpins[i]->setValue(-100);
        mLowerBoundLayout->addWidget(mLowerBoundSpins[i]);
    }


    mDim = dim;
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
    mOptimizer->runFor( ui->runForSpin->value(), ui->graphCheck->checkState() == Qt::Checked);
    plotData();

    std::cout << "Best: " <<  mOptimizer->getBestValue() << std::endl;
}
void MainWindow::initButtonPressed()
{

    createOptFunc();
    mOptimizer->initializeOptimizer(mOptFunc);
    ui->plotXDimSpin->setMaximum(mOptimizer->getDimension() - 1);
    ui->plotYDimSpin->setMaximum(mOptimizer->getDimension() - 1);
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


