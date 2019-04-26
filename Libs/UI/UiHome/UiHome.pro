#-------------------------------------------------
#
# Project created by QtCreator 2019-01-21T14:25:34
#
#-------------------------------------------------

include (../UI.pri)
INCLUDEPATH+=$${PWD}/HomeConfig


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
    graphhome136.cpp \
    homeconfigbinding139.cpp \
    HomeConfig/ihomeconfigitem.cpp \
    HomeConfig/homeconfigmanager.cpp \
    HomeConfig/homeitem0.cpp

HEADERS  += uihome.h \
    uihome_global.h \
    graphhome136.h \
    homeconfigbinding139.h \
    HomeConfig/ihomeconfigitem.h \
    HomeConfig/homeconfigmanager.h \
    HomeConfig/homeitem0.h

FORMS    += uihome.ui \
    graphhome136.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

TRANSLATIONS    += ch_uihome.ts en_uihome.ts
