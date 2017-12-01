QT += core
QT -= gui

TARGET = src
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH+=../../Libs/Com/Communication\
             ../../Libs/GTUtils/QtTreeManager\
             ../../Libs/GTUtils

CONFIG(debug, debug|release){
    COMMONSRC_OUT_PATH=$${PWD}/../../build/debug/bin
    LIBPATH=$${COMMONSRC_OUT_PATH}
    LIBS +=-lCommunicationd\
           -lGTUtilsd
    TARGET = CommonSrcAppd
} else{
    COMMONSRC_OUT_PATH=$${PWD}/../../build/release/bin
    LIBPATH=$${COMMONSRC_OUT_PATH}
#    LIBS +=$${COMMONSRC_OUT_PATH}\Communication.lib\
#           $${COMMONSRC_OUT_PATH}\GTUtils.lib
    LIBS +=-lCommunication\
           -lGTUtils
    TARGET = CommonSrcApp
}
DESTDIR =$${COMMONSRC_OUT_PATH}

SOURCES += main.cpp \
    Kernel/idevreadwriter.cpp \
    Kernel/linksocket.cpp \
    Kernel/ictrboard.cpp \
    Kernel/ipwrboard.cpp \
    Kernel/axistreemap.cpp \
    Kernel/idspmap.cpp \
    Kernel/sevdevice.cpp \
    Kernel/iuicontroler.cpp \
    Kernel/sevuicontroler.cpp \
    Kernel/sdassembly.cpp \
    Kernel/globalconfig.cpp \
    Kernel/gconfigreadwriter.cpp \
    Kernel/devtextrwriter.cpp \
    Kernel/devcomrwriter.cpp \
    Kernel/deviceconfig.cpp


HEADERS += \
    sdtglobaldef.h \
    Kernel/idevreadwriter.h \
    Kernel/linksocket.h \
    Kernel/ictrboard.h \
    Kernel/ipwrboard.h \
    Kernel/axistreemap.h \
    Kernel/idspmap.h \
    Kernel/sevdevice.h \
    Kernel/iuicontroler.h \
    Kernel/sevuicontroler.h \
    Kernel/sdassembly.h \
    Kernel/globalconfig.h \
    Kernel/gconfigreadwriter.h \
    Kernel/devtextrwriter.h \
    Kernel/devcomrwriter.h \
    Kernel/deviceconfig.h



