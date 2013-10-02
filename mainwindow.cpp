#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDateTime>
#include <stdlib.h>
#include <vector>
#include <utility>


#include "population.h"
#include "canonpsopopulation.h"
#include "optimizationFunctions.h"
#include "canonpsocontroller.h"
#include "constrictionpsocontroller.h"
#include "barebonespsocontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mOptimizer(0),
    mOptFunc(0),
    mUpperBoundSpins(0),
    mUpperBoundLayout(0),
    mLowerBoundSpins(0),
    mLowerBoundLayout(0),
    mScaleSpins(0),
    mScaleLayout(0),
    mTranslateSpins(0),
    mTranslateLayout(0),
    mVisualize(false),
    mVisualizeGraphs(0),
    mVisXYRes(200),
    mVisZRes(100),
    mDim(2)
{
    ui->setupUi(this);
    srand(QDateTime::currentMSecsSinceEpoch());

    ui->optimizerCombo->addItem("Canonical PSO", "canon");
    ui->optimizerCombo->addItem("Constriction Coefficient PSO", "constriction");
    ui->optimizerCombo->addItem("BareBones Particle Swarm", "barebones");

    ui->functionCombo->addItem("Sphere Function", "sphere");
    ui->functionCombo->addItem("Generalized Rastrigin Function", "gRastrigin");
    ui->functionCombo->addItem("Rosenbrock Function", "rosenbrock");
    ui->functionCombo->addItem("Beale's Function", "beal");
    ui->functionCombo->addItem("Goldstein-Price Function", "gp");
    ui->functionCombo->addItem("Griewank Function", "grie");
    //ui->functionCombo->addItem("Schwefel Function", "schw");

    QObject::connect(ui->stepButton, &QAbstractButton::clicked, this, &MainWindow::stepButtonPressed);
    QObject::connect(ui->initButton, &QAbstractButton::clicked, this, &MainWindow::initButtonPressed);
    QObject::connect(ui->zoomBoundsButton, &QAbstractButton::clicked, this, &MainWindow::zoomBoundsButtonPressed);
    QObject::connect(ui->zoomFitButton, &QAbstractButton::clicked, this, &MainWindow::zoomFitButtonPressed);
    QObject::connect(ui->clearOutputButton, &QAbstractButton::clicked, this, &MainWindow::clearButtonPressed);


    QObject::connect(ui->runForButton, &QAbstractButton::clicked, this, &MainWindow::runForButtonPressed);

    QObject::connect(ui->plotXDimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::plotData);
    QObject::connect(ui->plotYDimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::plotData);

    QObject::connect(ui->logCheckBox, &QCheckBox::clicked, this, &MainWindow::updateGraphScale);

    QObject::connect(ui->visCheck, &QCheckBox::clicked, this, &MainWindow::visDataCheckPressed);

    QObject::connect(ui->optimizerCombo,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::optComboChanged);

    QObject::connect(ui->functionCombo,
            static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this,
            &MainWindow::functionComboChanged);

    QObject::connect(ui->dimSpin, &QAbstractSpinBox::editingFinished, this, &MainWindow::dimSpinChanged);


    qout = new QDebugStream(std::cout, ui->textEdit);  


    mUpperBoundLayout = new QVBoxLayout(ui->upperBoundsScroll);
    mLowerBoundLayout = new QVBoxLayout(ui->lowerBoundsScroll);
    mScaleLayout = new QVBoxLayout(ui->scaleScroll);
    mTranslateLayout = new QVBoxLayout(ui->translateScroll);

    ui->plot->addLayer("items");
    ui->plot->addLayer("bkgrnd");

    optComboChanged();
    dimSpinChanged();
    createOptFunc();
}

void MainWindow::initVisGraphs()
{
    if(!mVisualize) return;
    if(mOptFunc == 0) return;
    if(mVisualizeGraphs != 0)
    {
        removeVisGraphs();
    }

    mVisualizeGraphs = new QCPGraph*[mVisZRes];

    int num = mVisXYRes*mVisXYRes;
    int xAxis = ui->plotXDimSpin->value();
    int yAxis = ui->plotYDimSpin->value();

    double xLowBound = mLowerBoundSpins[xAxis]->value(); //cheating, might bug out
    double xUpBound = mUpperBoundSpins[xAxis]->value();
    double xPosStep = (xUpBound - xLowBound)/mVisXYRes;

    double yLowBound = mLowerBoundSpins[yAxis]->value();
    double yUpBound = mUpperBoundSpins[yAxis]->value();
    double yPosStep = (yUpBound - yLowBound)/mVisXYRes;

    double sSize = ui->plot->size().width();
    if(sSize > ui->plot->size().height())
        sSize = ui->plot->size().height();
    sSize /= mVisXYRes + mVisXYRes;

    int cMinH = 280;
    int cMaxH = -100;
    int cHStep = (cMaxH - cMinH) / mVisZRes;
    int cMinSatur = 255;
    int cMaxSatur = 255;
    int cSaturStep = (cMaxSatur - cMinSatur) / mVisZRes;
    int cVal = 255;

    for(int i = 0 ; i < mVisZRes ; ++i)
    {
        mVisualizeGraphs[i] = ui->plot->addGraph();
        mVisualizeGraphs[i]->setLineStyle( QCPGraph::lsNone );
        mVisualizeGraphs[i]->setScatterStyle( QCP::ssSquare );
        mVisualizeGraphs[i]->setScatterSize( sSize );
        mVisualizeGraphs[i]->setPen( QPen( QColor::fromHsv(cMinH + (i*cHStep), cMinSatur + (i * cSaturStep) , cVal),  Qt::SolidPattern) );
    }



    double* values = new double[num * mDim];
    for(int j = 0 ; j < mVisXYRes ; ++j)
    {
        for(int i = 0 ; i < mVisXYRes; ++i)
        {
            for(int k = 0 ; k < mDim ; ++k)
            {
                values[(j * mVisXYRes * mDim) + (i * mDim) + k] = 0;//project other axis to zero
            }
            values[(j * mVisXYRes * mDim) + (i * mDim) + xAxis] = xLowBound + (j * xPosStep);
            values[(j * mVisXYRes * mDim) + (i * mDim) + yAxis] = yLowBound + (i * yPosStep);
        }
    }

    std::vector< std::pair< double, double* > > tempData;
    std::pair<double,double*> tempP;

    double minValue = mOptFunc->getOptimalValue();
    double maxValue = minValue;

    double meanVal = 0;

    for(int j = 0 ; j < mVisXYRes ; ++j)
    {
        for(int i = 0 ; i < mVisXYRes; ++i)
        {
            tempP.second = &(values[(j * mVisXYRes * mDim) + (i*mDim)]);
            mOptFunc->evaluate(tempP.second, tempP.first );
            meanVal += tempP.first;
            if(tempP.first > maxValue)
                maxValue = tempP.first;
            tempP.first *= -1; //hack to get minheap easy
            tempData.push_back(tempP);

        }
    }

    meanVal /= num;

    std::make_heap(tempData.begin(), tempData.end());

    double valBucketStep = (meanVal - minValue) * 2 / (double)mVisZRes;
    tempP.second=0;
    tempP.first=minValue;

    ui->plot->setCurrentLayer("bkgrnd");

    for(int i = 0 ; i < mVisZRes ; ++i )
    {
        while(tempData.size()>0)
        {
            tempP = tempData.front();

            std::pop_heap(tempData.begin(), tempData.end());
            tempData.pop_back();

            if((tempP.first * -1) > minValue+(valBucketStep*(i)) && i < mVisZRes - 1 )
            {
                mVisualizeGraphs[i+1]->addData( tempP.second[xAxis], tempP.second[yAxis] );
                break;
            }
            else
            {
                mVisualizeGraphs[i]->addData( tempP.second[xAxis], tempP.second[yAxis] );
            }
        }
    }

    delete[] values;


}

void MainWindow::removeVisGraphs()
{
    if(mVisualizeGraphs == 0) return;

    for(int i = 0 ; i < mVisZRes ; ++i)
    {
        ui->plot->removeGraph(mVisualizeGraphs[i]);//the remove function deletes the data I believe
    }
    delete[] mVisualizeGraphs;
    mVisualizeGraphs = 0;
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
    else if ( QString::compare( optName, "barebones" ) == 0) // Constriction PSO
    {
        mOptimizer = new BareBonesPSOController();
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

    bool applyScale = false;
    for(int i = 0 ; i < dim ; ++i)
    {
        if(mScaleSpins[i]->value() != 1)
        {
            applyScale = true;
            break;
        }
    }
    double* scale = 0;
    if(applyScale)
    {
        scale = new double[dim];
        for(int i = 0 ; i < dim ; ++i)
        {
            scale[i] = mScaleSpins[i]->value();
        }
    }

    bool applyTranslation = false;              //pass null pointer if no scaling needed
    for(int i = 0 ; i < dim ; ++i)
    {
        if(mTranslateSpins[i]->value() != 0)
        {
            applyTranslation = true;
            break;
        }
    }
    double* translation = 0;                    //pass null pointer if no translation needed
    if(applyTranslation)
    {
        translation = new double[dim];
        for(int i = 0 ; i < dim ; ++i)
        {
            translation[i] = mTranslateSpins[i]->value();
        }
    }

    if( QString::compare( optFuncName, "sphere") == 0 )
    {
        mOptFunc = new SphereFunction(dim, bounds, translation, scale);
    }
    else if( QString::compare( optFuncName, "gRastrigin") == 0 )
    {
        mOptFunc = new GeneralizedRastriginFunction(dim, bounds, translation, scale);
    }
    else if( QString::compare( optFuncName, "rosenbrock") == 0 )
    {
        mOptFunc = new RosenbrockFunction(bounds, translation, scale);
    }
    else if( QString::compare( optFuncName, "beal") == 0 )
    {
        mOptFunc = new BealesFunction(bounds,translation,scale);
    }
    else if( QString::compare( optFuncName, "gp") == 0 )
    {
        mOptFunc = new GoldsteinPriceFunction(bounds, translation, scale);
    }
    else if( QString::compare( optFuncName, "grie") == 0 )
    {
        mOptFunc = new GriewankFunction(dim, bounds, translation, scale);
    }
//    else if( QString::compare( optFuncName, "schw") == 0 )
//    {
//        mOptFunc = new SchwefelFunction(dim, bounds, translation, scale);
//    }
    else
    {
        mOptFunc = 0;
    }

    initVisGraphs();
}



void MainWindow::initPlot()
{
    if(mOptimizer==0) return;
    mOptimizer->removeGraphs(ui->plot, ui->graph);
    //ui->plot->clearGraphs();
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

    ui->plot->setCurrentLayer("items");
    mOptimizer->plotData(xDim, yDim);
    mOptimizer->graphValues();
    ui->graph->xAxis->setRange(0, mOptimizer->getGenNum() + 1);
    ui->plot->replot();
    ui->graph->replot();
}

void MainWindow::clearButtonPressed()
{
    ui->textEdit->clear();
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

void MainWindow::visDataCheckPressed()
{
    if(ui->visCheck->checkState() == Qt::Checked)
    {
        mVisualize = true;
        initVisGraphs();
    }
    else
    {
        mVisualize = false;
        removeVisGraphs();
    }
    ui->plot->replot();

}

void MainWindow::optComboChanged()
{
    QString what = ui->optimizerCombo->itemData(ui->optimizerCombo->currentIndex()).toString();
    createOptimizer(what);//TODO: add protection from seqfault caused by changing combo box then not initializing afterwards
}

void MainWindow::functionComboChanged()
{
    QString optFuncName = ui->functionCombo->itemData(ui->functionCombo->currentIndex()).toString();

    //Set input constraints
    //todo: make this better somehow


    if( QString::compare( optFuncName, "rosenbrock") == 0 )
    {
        ui->dimSpin->setEnabled(false);
        ui->dimSpin->setValue(2);
    }
    else if( QString::compare( optFuncName, "beale") == 0 )
    {
        ui->dimSpin->setEnabled(false);
        ui->dimSpin->setValue(2);
    }
    else if( QString::compare( optFuncName, "gp") == 0 )
    {
        ui->dimSpin->setEnabled(false);
        ui->dimSpin->setValue(2);
    }
    else
    {
        ui->dimSpin->setEnabled(true);
    }
    dimSpinChanged();
}

void MainWindow::dimSpinChanged()
{
    int dim = ui->dimSpin->value();
    if(dim < 2) return;

    double* defaultBounds = 0;

    QString optFuncName = ui->functionCombo->itemData(ui->functionCombo->currentIndex()).toString();

    if( QString::compare( optFuncName, "sphere") == 0 )
    {
         defaultBounds = SphereFunction::getDefaultBounds(dim);
    }
    else if( QString::compare( optFuncName, "gRastrigin") == 0 )
    {
         defaultBounds = GeneralizedRastriginFunction::getDefaultBounds(dim);
    }
    else if( QString::compare( optFuncName, "rosenbrock") == 0 )
    {
        defaultBounds = RosenbrockFunction::getDefaultBounds(dim);
    }
    else if( QString::compare( optFuncName, "beale") == 0 )
    {
        defaultBounds = BealesFunction::getDefaultBounds(dim);
    }
    else if( QString::compare( optFuncName, "rosenbrock") == 0 )
    {
        defaultBounds = GoldsteinPriceFunction::getDefaultBounds(dim);
    }
    else if( QString::compare( optFuncName, "grie") == 0 )
    {
        defaultBounds = GriewankFunction::getDefaultBounds(dim);
    }
//    else if( QString::compare( optFuncName, "schw") == 0 )
//    {
//        defaultBounds = SchwefelFunction::getDefaultBounds(dim);
//    }
    else
    {
        defaultBounds = new double[dim*2];
        for(int i = 0 ; i < dim ; ++i)
        {
            defaultBounds[i] = -10;
            defaultBounds[i + dim] = 10;
        }
    }

    if(mUpperBoundSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mUpperBoundSpins[i];

        delete[] mUpperBoundSpins;
    }

    mUpperBoundSpins = new QDoubleSpinBox*[dim];
    for(int i = 0 ; i < dim ; ++i)
    {
        mUpperBoundSpins[i] = new QDoubleSpinBox();
        mUpperBoundSpins[i]->setMaximum(9999999);
        mUpperBoundSpins[i]->setMinimum(-9999999);
        mUpperBoundSpins[i]->setValue(defaultBounds[i + dim]);
        mUpperBoundLayout->addWidget(mUpperBoundSpins[i]);
    }

    if(mLowerBoundSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mLowerBoundSpins[i];

        delete[] mLowerBoundSpins;
    }

    mLowerBoundSpins = new QDoubleSpinBox*[dim];
    for(int i = 0 ; i < dim ; ++i)
    {
        mLowerBoundSpins[i] = new QDoubleSpinBox();
        mLowerBoundSpins[i]->setMaximum(9999999);
        mLowerBoundSpins[i]->setMinimum(-9999999);
        mLowerBoundSpins[i]->setValue(defaultBounds[i]);
        mLowerBoundLayout->addWidget(mLowerBoundSpins[i]);
    }

    delete[] defaultBounds;

    if(mScaleSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mScaleSpins[i];

        delete[] mScaleSpins;
    }

    mScaleSpins = new QDoubleSpinBox*[dim];
    for(int i = 0 ; i < dim ; ++i)
    {
        mScaleSpins[i] = new QDoubleSpinBox();
        mScaleSpins[i]->setMaximum(9999999);
        mScaleSpins[i]->setMinimum(0.0001);
        mScaleSpins[i]->setValue(1);
        mScaleSpins[i]->setSingleStep(0.1);
        mScaleLayout->addWidget(mScaleSpins[i]);
    }

    if(mTranslateSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mTranslateSpins[i];

        delete[] mTranslateSpins;
    }

    mTranslateSpins = new QDoubleSpinBox*[dim];
    for(int i = 0 ; i < dim ; ++i)
    {
        mTranslateSpins[i] = new QDoubleSpinBox();
        mTranslateSpins[i]->setMaximum(9999999);
        mTranslateSpins[i]->setMinimum(-999999);
        mTranslateSpins[i]->setValue(0);
        mTranslateSpins[i]->setSingleStep(1);
        mTranslateLayout->addWidget(mTranslateSpins[i]);
    }

    ui->plotXDimSpin->setMaximum(dim);
    ui->plotYDimSpin->setMaximum(dim);

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
    initVisGraphs();
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
{   //TODO: several things not deleted properly due to laziness, fix when not lazy (hahaha yeah right)
    if(mUpperBoundSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mUpperBoundSpins[i];

        delete[] mUpperBoundSpins;
    }
    if(mLowerBoundSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mLowerBoundSpins[i];

        delete[] mLowerBoundSpins;
    }
    if(mScaleSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mScaleSpins[i];

        delete[] mScaleSpins;
    }
    if(mTranslateSpins != 0)
    {
        for(int i = 0 ; i < mDim ; ++i)
            delete mTranslateSpins[i];

        delete[] mTranslateSpins;
    }
    if(mOptimizer != 0) mOptimizer->removeGraphs(ui->plot, ui->graph);
    delete mOptimizer;
    delete qout;
    delete ui;


}


