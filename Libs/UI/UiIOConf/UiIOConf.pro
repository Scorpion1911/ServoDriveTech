#-------------------------------------------------
#
# Project created by QtCreator 2018-12-17T17:17:42
#
#-------------------------------------------------
include (../UI.pri)
QWT=$${PWD}/../../ThirdParty/qwt6.1.2
INCLUDEPATH+=$${QWT}/includes

QT       += widgets

QT       -= gui

TEMPLATE = lib

CONFIG(debug, debug|release){
    LIBS+=$${QWT}/libs/qwtd.lib
    TARGET = UiIOConfd
} else{
    LIBS+=$${QWT}/libs/qwt.lib
    TARGET = UiIOConf
}

DEFINES += UIIOCONF_LIBRARY

SOURCES += uiioconf.cpp \
    graphioconf136.cpp \
    singleiowidget.cpp \
    graphioconf138.cpp \
    maskwheelcombobox.cpp

HEADERS += uiioconf.h\
        uiioconf_global.h \
    graphioconf136.h \
    singleiowidget.h \
    graphioconf138.h \
    maskwheelcombobox.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    uiioconf.ui \
    graphioconf136.ui \
    singleiowidget.ui \
    graphioconf138.ui

RESOURCES +=

TRANSLATIONS    += ch_uiioconf.ts en_uiioconf.ts
