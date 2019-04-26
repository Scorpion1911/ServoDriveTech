#-------------------------------------------------
#
# Project created by QtCreator 2017-12-08T13:54:40
#
#-------------------------------------------------
include (../UI.pri)
QWT=$${PWD}/../../ThirdParty/qwt6.1.2
INCLUDEPATH+=$${QWT}/includes\
             $${PWD}/EncConfig

QT       += widgets

QT       -= gui

TARGET = UiEncoder
TEMPLATE = lib

DEFINES += UIENCODER_LIBRARY

CONFIG(debug, debug|release){
    LIBS+=$${QWT}/libs/qwtd.lib
    TARGET = UiEncoderd
} else{
    LIBS+=$${QWT}/libs/qwt.lib
    TARGET = UiEncoder
}

SOURCES += uiencoder.cpp \
    graphencoder130.cpp \
    ./EncConfig/encconfigmanage.cpp \
    ./EncConfig/iencconfigitem.cpp \
    EncConfig/encconfigduomoitem.cpp \
    EncConfig/encconfighaidehanitem.cpp \
    EncConfig/encconfignikangitem.cpp \
    EncConfig/encconfigsanxieitem.cpp \
    EncConfig/encconfigsongxiaitem.cpp \
    EncConfig/encconfigyaskawaitem.cpp \
    linenumberbinding.cpp \
    encconfigbinding130.cpp \
    encconfigbinding126.cpp \
    graphencoder126.cpp \
    encconfigbinding136.cpp \
    graphencoder136.cpp \
    EncConfig/encconfigsinz.cpp \
    EncConfig/encconfigsinhiperface.cpp \
    EncConfig/encconfigsinendat.cpp \
    EncConfig/encconfigrotation.cpp \
    EncConfig/encconfigabz.cpp \
    EncConfig/encconfighechuanitem.cpp

HEADERS += uiencoder.h\
        uiencoder_global.h \
    graphencoder130.h \
    ./EncConfig/encconfigmanage.h \
    ./EncConfig/iencconfigitem.h \
    EncConfig/encconfigduomoitem.h \
    EncConfig/encconfighaidehanitem.h \
    EncConfig/encconfignikangitem.h \
    EncConfig/encconfigsanxieitem.h \
    EncConfig/encconfigsongxiaitem.h \
    EncConfig/encconfigyaskawaitem.h \
    EncConfig/EncConfig \
    linenumberbinding.h \
    encconfigbinding130.h \
    encconfigbinding126.h \
    graphencoder126.h \
    encconfigbinding136.h \
    graphencoder136.h \
    EncConfig/encconfigsinz.h \
    EncConfig/encconfigsinhiperface.h \
    EncConfig/encconfigsinendat.h \
    EncConfig/encconfigrotation.h \
    EncConfig/encconfigabz.h \
    EncConfig/encconfighechuanitem.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    uiencoder.ui \
    graphencoder130.ui \
    graphencoder126.ui \
    graphencoder136.ui

TRANSLATIONS    += ch_uiencoder.ts en_uiencoder.ts
