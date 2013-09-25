#-------------------------------------------------
#
# Project created by QtCreator 2013-05-16T13:10:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4){
 QT += widgets
    QT += printsupport
}
TARGET = ParticleSwarms
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    optimizercontroller.cpp\
    population.cpp \
    particlevpopulation.cpp \
    canonpsopopulation.cpp \
    constrictionpsopopulation.cpp \
    canonpsocontroller.cpp \
    constrictionpsocontroller.cpp

HEADERS  += mainwindow.h \
    optimizationFunctions.h \
    QDebugStream.h \
    qcustomplot.h \
    optimizercontroller.h\
    population.h \
    particlevpopulation.h \
    canonpsopopulation.h \
    constrictionpsopopulation.h \
    canonpsocontroller.h \
    constrictionpsocontroller.h

FORMS    += mainwindow.ui
