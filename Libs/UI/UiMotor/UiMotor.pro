#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T13:53:43
#
#-------------------------------------------------
include (../UI.pri)

QT       += widgets sql
QT       -= gui

INCLUDEPATH += $${PWD}/MotorDBManager\

TEMPLATE = lib

DEFINES += UIMOTOR_LIBRARY

CONFIG(debug, debug|release){
    TARGET = UiMotord
} else{
    TARGET = UiMotor
}

SOURCES += uimotor.cpp \
    graphmotor129.cpp \
    graphmotor130.cpp \
    MotorDBManager/motordbui.cpp \
    MotorDBManager/motordbmanager.cpp \

HEADERS += uimotor.h\
        uimotor_global.h \
    graphmotor129.h \
    graphmotor130.h \
    MotorDBManager/motordbui.h \
    MotorDBManager/motordbmanager.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    uimotor.ui \
    graphmotor129.ui \
    graphmotor130.ui \
    MotorDBManager/motordbui.ui

TRANSLATIONS    += ch_uimotor.ts en_uimotor.ts
