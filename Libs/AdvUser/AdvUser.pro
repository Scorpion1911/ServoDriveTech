#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T16:15:50
#
#-------------------------------------------------

include(../Libs.pri)
INCLUDEPATH+=$${PWD}/../GTUtils\
             $${PWD}/../GTUtils/QtTreeManager\
             $${PWD}/../GTUtils/BitItemHelper\
             $${PWD}/../GTUtils/FolderCompressor\
             $${PWD}/../DbManager\
             $${PWD}/../SDKernel\
             $${PWD}/../Com\
             $${PWD}/../Com/Communication\

QT       += widgets core sql

QT       -= gui

TEMPLATE = lib

DEFINES += ADVUSER_LIBRARY

CONFIG(debug, debug|release){
    ADVUSR_OUT_PATH = $${APP_BUILD_PATH}/debug/bin
    LIBS +=$${ADVUSR_OUT_PATH}/GTUtilsd.lib\
           $${ADVUSR_OUT_PATH}/SDKerneld.lib\
            $${PWD}/../../build/debug/bin/ServoDriverComDlld.lib\
           $${ADVUSR_OUT_PATH}/DbManagerd.lib
    TARGET = AdvUserd
} else{
    ADVUSR_OUT_PATH=$${APP_BUILD_PATH}/release/bin
    LIBS +=$${ADVUSR_OUT_PATH}/GTUtils.lib\
           $${ADVUSR_OUT_PATH}/SDKernel.lib\
            $${PWD}/../../build/debug/bin/ServoDriverComDll.lib\
           $${ADVUSR_OUT_PATH}/DbManager.lib
    TARGET = AdvUser
}

DESTDIR =$${ADVUSR_OUT_PATH}

SOURCES += iadvuser.cpp \
    advusercheck.cpp \
    advusercontainer.cpp \
    advusermask.cpp \
    advusercompress.cpp \
    advuserfirmwaresegmentflash.cpp

HEADERS += iadvuser.h\
        advuser_global.h \
    iadvuser_p.h \
    advusercheck.h \
    advusercontainer.h \
    advusermask.h \
    advusercompress.h \
    advuserfirmwaresegmentflash.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    advusercheck.ui \
    advusermask.ui \
    advusercompress.ui \
    advuserfirmwaresegmentflash.ui

TRANSLATIONS    += ch_advuser.ts en_advuser.ts
