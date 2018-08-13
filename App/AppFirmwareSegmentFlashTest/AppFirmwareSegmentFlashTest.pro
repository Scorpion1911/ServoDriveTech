#-------------------------------------------------
#
# Project created by QtCreator 2018-07-27T11:05:20
#
#-------------------------------------------------
include(../App.pri)

QT       += core gui qml designer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

INCLUDEPATH += $${PWD}/../../Libs/Com/ServoDriverComDll/DllCom\
               $${PWD}/../../Libs/Com/ServoDriverComDll/NetCom/include\
               $${PWD}/../../Libs/SDKernel

CONFIG(debug,debug|release){
    LIBS += $${PWD}/../../build/debug/bin/ServoDriverComDlld.lib
    TARGET = FirmSegmentFlashTestd
}else{
    LIBS += $${PWD}/../../build/release/bin/ServoDirverComDll.lib
    TARGET = FirmSegmentFlashTest
}


SOURCES += main.cpp\
        firmwaresegmentflashdialog.cpp\


HEADERS  += firmwaresegmentflashdialog.h\


FORMS    += firmwaresegmentflashdialog.ui
