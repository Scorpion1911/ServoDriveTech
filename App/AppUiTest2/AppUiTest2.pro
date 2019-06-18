#-------------------------------------------------
#
# Project created by QtCreator 2019-05-15T15:24:56
#
#-------------------------------------------------
include(../App.pri)
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release){
  TARGET = AppUiTest2d
} else{
  TARGET = AppUiTest2
}
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
