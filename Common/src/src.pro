 QT += core widgets
QT -= gui

TARGET = src
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH+=../../Libs/Com/Communication\
              ../../Libs/GTUtils/QtTreeManager\
              ../../Libs/GTUtils/DeviceIdHelper\
              ../../Libs/GTUtils/SelfBuilder\
              ../../Libs/GTUtils\
              ../../Libs/UI\
              ../../Libs/UI/IUiWidget\
              ../../Libs/SDKernel\
              ../../Libs/Option\
              ../../Libs/PlotUnit/PlotUnitGraph\
              ../../Libs/PlotUnit/IPlotUnit\
              $${PWD}/../../Libs/PlotCurvePluginFramework/ICurve\
              $${PWD}/UiFactory\
              $${PWD}/Kernel\
              $${PWD}/SdtGlobal\
              $${PWD}/HalfAutoDialog

CONFIG(debug, debug|release){
    COMMONSRC_OUT_PATH=$${PWD}/../../build/debug/bin
    QML_IMPORT_PATH +=$${PWD}/../../build/debug/custom/option/qmlstyle
    LIBPATH=$${COMMONSRC_OUT_PATH}
    LIBS +=-lCommunicationd\
          -lGTUtilsd\
          -lIUiWidgetd\
          -lUiBraked\
          -lUiCurrentd\
          -lUiEncoderd\
          -lUiFLASHd\
          -lUiIOd\
          -lUiMotord\
          -lUiPlotd\
          -lUiPositiond\
          -lUiPowerd\
          -lUiRAMd\
          -lUiStatusd\
          -lUiVelocityd\
          -lSDKerneld\
          -lOptiond\
          -lPlotUnitGraphd\
          -lIPlotUnitd\
          -lUiIOConfd\
          -lUiIOAnalogd\
          -lUiHomed
    TARGET = CommonSrcAppd
} else{
    COMMONSRC_OUT_PATH=$${PWD}/../../build/release/bin
    QML_IMPORT_PATH +=$${PWD}/../../build/release/custom/option/qmlstyle
    LIBPATH=$${COMMONSRC_OUT_PATH}
#    LIBS +=$${COMMONSRC_OUT_PATH}\Communication.lib\
#           $${COMMONSRC_OUT_PATH}\GTUtils.lib
    LIBS +=-lCommunication\
          -lGTUtils\
          -lIUiWidget\
          -lUiBrake\
          -lUiCurrent\
          -lUiEncoder\
          -lUiFLASH\
          -lUiIO\
          -lUiMotor\
          -lUiPlot\
          -lUiPosition\
          -lUiPower\
          -lUiRAM\
          -lUiStatus\
          -lUiVelocity\
          -lSDKernel\
          -lOption\
          -lPlotUnitGraph\
          -lIPlotUnit\
          -lUiIOConf\
          -lUiIOAnalog\
          -lUiHome
    TARGET = CommonSrcApp
}
DESTDIR =$${COMMONSRC_OUT_PATH}

SOURCES += main.cpp \
    Kernel/idevreadwriter.cpp \
    Kernel/iuicontroler.cpp \
    Kernel/sevuicontroler.cpp \
    Kernel/sdassembly.cpp \
    Kernel/globalconfig.cpp \
    Kernel/gconfigreadwriter.cpp \
    Kernel/devtextrwriter.cpp \
    Kernel/devcomrwriter.cpp \
    Kernel/deviceconfig.cpp \
    UiFactory/uifactory.cpp \
    Kernel/globaluicontroler.cpp \
    test.c \
    SdtGlobal/sdtglobaldef.cpp \
    Kernel/sdterror.cpp \
    ledalarm.cpp \
    HalfAutoDialog/halfautodialog.cpp


HEADERS += \
    Kernel/idevreadwriter.h \
    Kernel/iuicontroler.h \
    Kernel/sevuicontroler.h \
    Kernel/sdassembly.h \
    Kernel/globalconfig.h \
    Kernel/gconfigreadwriter.h \
    Kernel/devtextrwriter.h \
    Kernel/devcomrwriter.h \
    Kernel/deviceconfig.h \
    UiFactory/uifactory.h \
    UiFactory/registerfunction.h \
    Kernel/globaluicontroler.h \
    test.h \
    SdtGlobal/sdtglobaldef.h \
    Kernel/sdterror.h \
    ledalarm.h \
    HalfAutoDialog/halfautodialog.h

FORMS += \
    HalfAutoDialog/halfautodialog.ui




