#include "graphhome136.h"
#include "ui_graphhome136.h"

#include "igraphhome_p.h"
#include "iuiwidget.h"
#include "uihome.h"
#include "sevdevice.h"

#include <QDebug>
#include <QTreeWidget>

class GraphHome136Private:public IGraphHomePrivate
{
  Q_DECLARE_PUBLIC(GraphHome136)
public:
  GraphHome136Private(){}
  ~GraphHome136Private(){}
};

GraphHome136::GraphHome136(QWidget *parent) :
    IGraphHome(*(new GraphHome136Private), parent),
    ui(new Ui::GraphHome136)
{
    ui->setupUi(this);
}

GraphHome136::~GraphHome136()
{
    delete ui;
}

void GraphHome136::syncTreeDataToUiFace()
{

}

void GraphHome136::setCustomVisitActive(IUiWidget *uiWidget)
{

}

void GraphHome136::setUiVersionName()
{

}

void GraphHome136::setupDataMappings()
{

}
