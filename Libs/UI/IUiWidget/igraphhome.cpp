#include "igraphhome.h"
#include "igraphhome_p.h"
#include <QDebug>

//IGraphHome::IGraphHome(QWidget *parent) : QWidget(parent)
//{

//}

IGraphHome::~IGraphHome()
{
  qDebug()<<"IGraphHome destruct-->";
}

IGraphHome::IGraphHome(IGraphHomePrivate &dd, QWidget *parent):IGraph(dd, parent)
{

}

