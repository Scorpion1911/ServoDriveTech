#-------------------------------------------------
#
# Project created by QtCreator 2017-12-01T11:14:00
#
#-------------------------------------------------

include(../Libs.pri)
UtilsTest=0
isEqual(UtilsTest,1){
  BUILD_PATH=$${PWD}/build/
}
isEqual(UtilsTest,0){
  BUILD_PATH=$${APP_BUILD_PATH}
}
INCLUDEPATH+=$${PWD}/QtTreeManager\
             $${PWD}/../Com/Communication\
             $${PWD}/SelfBuilder\
             $${PWD}/BitItemHelper\
             $${PWD}/CmdManager\

QT +=designer
QT       -= gui

TEMPLATE = lib

DEFINES += GTUTILS_LIBRARY

CONFIG(debug, debug|release){
    LIBS +=$${APP_BUILD_PATH}/debug/bin/Communicationd.lib
    UTILS_OUT_PATH=$${BUILD_PATH}/debug/bin
    TARGET = GTUtilsd
} else{
    LIBS +=$${APP_BUILD_PATH}/release/bin/Communication.lib
    UTILS_OUT_PATH=$${BUILD_PATH}/release/bin
    TARGET = GTUtils
}
DESTDIR =$${UTILS_OUT_PATH}


SOURCES += \
    QtTreeManager/qttreemanager.cpp \
    gtutils.cpp \
    DeviceIdHelper/deviceidhelper.cpp \
    SelfBuilder/selfbuilder.cpp \
    BitItemHelper/bititemhelper.cpp \
    CmdManager/cmdmanager.cpp \
    ../../Common/src/Kernel/sdterror.cpp \
    FolderCompressor/FolderCompressor.cpp \
    AutoTurnning/autoturnning.cpp \
    PixmapWidget/pixmapwidget.cpp

HEADERS +=\
    QtTreeManager/qttreemanager.h \
    gtutils_global.h \
    gtutils.h \
    DeviceIdHelper/deviceidhelper.h \
    SelfBuilder/selfbuilder.h \
    BitItemHelper/bititemhelper.h \
    CmdManager/cmdmanager.h \
    ../../Common/src/Kernel/sdterror.h \
    FolderCompressor/FolderCompressor.h \
    PixmapWidget/pixmapwidget.h \
    AutoTurnning/autoturnning.h \
    PixmapWidget/pixmapwidget_p.h \
    PixmapWidget/pixmapwidget.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

TRANSLATIONS    += ch_gtutils.ts en_gtutils.ts
