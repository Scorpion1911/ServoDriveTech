#-------------------------------------------------
#
# Project created by QtCreator 2019-03-22T17:06:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += $${PWD}/../../Libs/GTUtils

TARGET = ParaFileConversion
TEMPLATE = app

#    LIB_PATH = $${PWD}/../../build/debug/bin
#    LIBS += $${LIB_PATH}/GTUtilsd.lib

CONFIG(debug debug|release){
    LIB_PATH = $${PWD}/../../build/debug/bin
    LIBS += $${LIB_PATH}/GTUtilsd.lib
}else{
    LIB_PATH = $${PWD}/../../build/release/bin
    LIBS += $${LIB_PATH}/GTUtils.lib
}

SOURCES += main.cpp\
        parafileconversion.cpp

HEADERS  += parafileconversion.h

FORMS    += parafileconversion.ui
