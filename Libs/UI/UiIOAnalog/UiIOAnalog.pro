#-------------------------------------------------
#
# Project created by QtCreator 2019-01-25T09:59:20
#
#-------------------------------------------------

include (../UI.pri)
QT       += widgets
QT       -= gui

TEMPLATE = lib

DEFINES += UIIOANALOG_LIBRARY

CONFIG(debug, debug|release){
    TARGET = UiIOAnalogd
} else{
    TARGET = UiIOAnalog
}

SOURCES +=\
        uiioanalog.cpp \
    graphioanalog136.cpp

HEADERS  += uiioanalog.h \
    graphioanalog136.h \
    uiioanalog_global.h

FORMS    += uiioanalog.ui \
    graphioanalog136.ui

unix {
    target.path = /usr/lib
    INSTALLS += target
}

TRANSLATIONS    += ch_uiioanalog.ts en_uiioanalog.ts
