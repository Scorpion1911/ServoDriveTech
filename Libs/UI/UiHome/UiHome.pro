#-------------------------------------------------
#
# Project created by QtCreator 2019-01-21T14:25:34
#
#-------------------------------------------------

include (../UI.pri)

QT       += widgets
QT       -= gui

TEMPLATE = lib

DEFINES += UIHOME_LIBRARY

CONFIG(debug, debug|release){
    TARGET = UiHomed
} else{
    TARGET = UiHome
}

SOURCES +=\
        uihome.cpp \
    graphhome136.cpp

HEADERS  += uihome.h \
    uihome_global.h \
    graphhome136.h

FORMS    += uihome.ui \
    graphhome136.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

TRANSLATIONS    += ch_uihome.ts en_uihome.ts
