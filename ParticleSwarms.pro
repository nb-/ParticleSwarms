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
    particlevpopulation.cpp \
    canonpsopopulation.cpp \
    constrictionpsopopulation.cpp \
    canonpsocontroller.cpp \
    constrictionpsocontroller.cpp \
    barebonespsopopulation.cpp \
    barebonespsocontroller.cpp \
    newdev1population.cpp \
    particlepopulation.cpp \
    population.cpp \
    newdev1controller.cpp \
    newdev2population.cpp \
    newdev2controller.cpp

HEADERS  += mainwindow.h \
    optimizationFunctions.h \
    QDebugStream.h \
    qcustomplot.h \
    optimizercontroller.h\
    particlevpopulation.h \
    canonpsopopulation.h \
    constrictionpsopopulation.h \
    canonpsocontroller.h \
    constrictionpsocontroller.h \
    barebonespsopopulation.h \
    barebonespsocontroller.h \
    randomhelper.h \
    newdev1population.h \
    particlepopulation.h \
    population.h \
    newdev1controller.h \
    newdev2population.h \
    newdev2controller.h

FORMS    += mainwindow.ui
