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
    HomeConfig/homeitem0.cpp \
    HomeConfig/homeitem1.cpp \
    HomeConfig/homeitem2.cpp \
    HomeConfig/homeitem3.cpp \
    HomeConfig/homeitem4.cpp \
    HomeConfig/homeitem5.cpp \
    HomeConfig/homeitem6.cpp \
    HomeConfig/homeitem7.cpp \
    HomeConfig/homeitem8.cpp \
    HomeConfig/homeitem9.cpp \
    HomeConfig/homeitem10.cpp \
    HomeConfig/homeitem11.cpp \
    HomeConfig/homeitem12.cpp \
    HomeConfig/homeitem13.cpp \
    HomeConfig/homeitem14.cpp \
    HomeConfig/homeitem15.cpp \
    HomeConfig/homeitem16.cpp \
    HomeConfig/homeitem17.cpp \
    HomeConfig/homeitem18.cpp \
    HomeConfig/homeitem19.cpp \
    HomeConfig/homeitem20.cpp \
    HomeConfig/homeitem21.cpp \
    HomeConfig/homeitem22.cpp \
    HomeConfig/homeitem23.cpp \
    HomeConfig/homeitem24.cpp \
    HomeConfig/homeitem25.cpp \
    HomeConfig/homeitem26.cpp \
    HomeConfig/homeitem27.cpp \
    HomeConfig/homeitem28.cpp \
    HomeConfig/homeitem29.cpp \
    HomeConfig/homeitem30.cpp \
    HomeConfig/homeitem31.cpp \
    HomeConfig/homeitem32.cpp \
    HomeConfig/homeitem33.cpp \
    HomeConfig/homeitem34.cpp \
    HomeConfig/homeitem35.cpp

HEADERS  += uihome.h \
    uihome_global.h \
    graphhome136.h \
    homeconfigbinding139.h \
    HomeConfig/ihomeconfigitem.h \
    HomeConfig/homeconfigmanager.h \
    HomeConfig/homeitem0.h \
    HomeConfig/HomeConfig \
    HomeConfig/homeitem1.h \
    HomeConfig/homeitem2.h \
    HomeConfig/homeitem3.h \
    HomeConfig/homeitem4.h \
    HomeConfig/homeitem5.h \
    HomeConfig/homeitem6.h \
    HomeConfig/homeitem7.h \
    HomeConfig/homeitem8.h \
    HomeConfig/homeitem9.h \
    HomeConfig/homeitem10.h \
    HomeConfig/homeitem11.h \
    HomeConfig/homeitem12.h \
    HomeConfig/homeitem13.h \
    HomeConfig/homeitem14.h \
    HomeConfig/homeitem15.h \
    HomeConfig/homeitem16.h \
    HomeConfig/homeitem17.h \
    HomeConfig/homeitem18.h \
    HomeConfig/homeitem19.h \
    HomeConfig/homeitem20.h \
    HomeConfig/homeitem21.h \
    HomeConfig/homeitem22.h \
    HomeConfig/homeitem23.h \
    HomeConfig/homeitem24.h \
    HomeConfig/homeitem25.h \
    HomeConfig/homeitem26.h \
    HomeConfig/homeitem27.h \
    HomeConfig/homeitem28.h \
    HomeConfig/homeitem29.h \
    HomeConfig/homeitem30.h \
    HomeConfig/homeitem31.h \
    HomeConfig/homeitem32.h \
    HomeConfig/homeitem33.h \
    HomeConfig/homeitem34.h \
    HomeConfig/homeitem35.h

FORMS    += uihome.ui \
    graphhome136.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

TRANSLATIONS    += ch_uihome.ts en_uihome.ts
