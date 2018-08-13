#include "igraphio.h"
#include "igraphio_p.h"
#include "boxitemmapping.h"
#include "iuiwidget.h"

#include <QDebug>

//iGraphIO::iGraphIO(QObject *parent) : QObject(parent)
//{

//}

IGraphIO::~IGraphIO() {
    qDebug()<<"IGraphIO destruct-->";
}

IGraphIO::IGraphIO(IGraphIOPrivate &dd, QWidget *parent):IGraph(dd, parent) {

}

