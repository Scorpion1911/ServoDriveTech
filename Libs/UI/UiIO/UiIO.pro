#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T13:57:31
#
#-------------------------------------------------
include (../UI.pri)

QT       += widgets

QT       -= gui


TEMPLATE = lib

DEFINES += UIIO_LIBRARY

CONFIG(debug, debug|release){
    TARGET = UiIOd
} else{
    TARGET = UiIO
}

SOURCES += uiio.cpp \
    graphio133.cpp \
    graphio129.cpp \
    graphio126.cpp

HEADERS += uiio.h\
        uiio_global.h \
    graphio133.h \
    graphio129.h \
    graphio126.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    uiio.ui \
    graphio133.ui \
    graphio129.ui \
    graphio126.ui

TRANSLATIONS    += ch_uiio.ts en_uiio.ts
