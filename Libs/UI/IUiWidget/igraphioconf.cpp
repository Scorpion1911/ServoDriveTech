#include "igraphioconf.h"
#include "boxitemmapping.h"
#include "iuiwidget.h"
#include "igraphioconf_p.h"

#include <QDebug>

IGraphIOConf::~IGraphIOConf() {
    qDebug()<<"IGraphIOConf destruct-->";
}

IGraphIOConf::IGraphIOConf(IGraphIOConfPrivate &dd, QWidget *parent):IGraph(dd, parent) {

}

