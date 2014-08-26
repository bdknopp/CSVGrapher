#-------------------------------------------------
#
# Project created by QtCreator 2014-08-22T11:22:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CSVGrapher
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    LineGraphView.cpp

HEADERS  += MainWindow.h \
    CSVFileException.h \
    LineGraphView.h

FORMS    += MainWindow.ui
