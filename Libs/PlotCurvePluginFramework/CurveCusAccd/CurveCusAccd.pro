#-------------------------------------------------
#
# Project created by QtCreator 2019-03-04T13:45:30
#
#-------------------------------------------------

include (../ctkplugins.pri)
INCLUDEPATH += $$PWD/../../GTUtils/MathToolbox\
               $$PWD/../../GTUtils
QT       -= gui
QT+=widgets

TEMPLATE = lib

CONFIG(debug,debug|release){
  BIN_PATH=$$PWD/../../../build/debug/bin
  LIBS+=$$BIN_PATH/GTUtilsd.lib
  TARGET = CurveCusAccd
}else{
  BIN_PATH=$$PWD/../../../build/release/bin
  LIBS+=$$BIN_PATH/GTUtils.lib
  TARGET = CurveCusAccd
}

DESTDIR=$$PLOT_PLUGINS_PATH/custom/HuiKong

DEFINES += CURVECUSACC_LIBRARY

SOURCES += \
    curvecusaccd.cpp \
    curvecusaccdactivator.cpp

HEADERS += \
    curvecusaccd.h \
    curvecusaccdactivator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    resource.qrc
