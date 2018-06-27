#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T13:55:34
#
#-------------------------------------------------
include (../UI.pri)

QT       += widgets

QT       -= gui

TEMPLATE = lib

DEFINES += UIVELOCITY_LIBRARY

CONFIG(debug, debug|release){
    TARGET = UiVelocityd
} else{
    TARGET = UiVelocity
}

SOURCES += uivelocity.cpp \
    graphvelocity129.cpp \
    graphvelocity133.cpp \
    velpid133.cpp

HEADERS += uivelocity.h\
        uivelocity_global.h \
    graphvelocity129.h \
    graphvelocity133.h \
    velpid133.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    uivelocity.ui \
    graphvelocity129.ui \
    velpid133.ui
