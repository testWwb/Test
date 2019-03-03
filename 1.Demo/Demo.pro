#-------------------------------------------------
#
# Project created by QtCreator 2017-08-31T13:04:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += printsupport

TARGET = Demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    mythread.h

FORMS    += mainwindow.ui
