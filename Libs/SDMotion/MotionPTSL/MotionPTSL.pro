#-------------------------------------------------
#
# Project created by QtCreator 2019-02-18T10:41:40
#
#-------------------------------------------------

include(../Motion.pri)
QT       += widgets

QT       -= gui

TEMPLATE = lib

CONFIG(debug, debug|release){
    TARGET = MotionPTSLd
} else{
    TARGET = MotionPTSL
}

DEFINES += MOTIONPTSL_LIBRARY

SOURCES += motionptsl.cpp \
    uimotionptsl.cpp \
    ptplanmotion.cpp

HEADERS += motionptsl.h\
        motionptsl_global.h \
    uimotionptsl.h \
    ptplanmotion.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    uimotionptsl.ui
