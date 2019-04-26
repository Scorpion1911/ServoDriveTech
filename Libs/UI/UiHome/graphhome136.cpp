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
    ui->groupBox_homingMode->setVisible(false);
    ui->label_2->setVisible(false);
    ui->doubleSpinBox_homeAcc->setVisible(false);
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
    Q_D(GraphHome136);
    d->m_versionName = "V136";
}

void GraphHome136::setupDataMappings()
{
    Q_D(GraphHome136);
    QList<QDoubleSpinBox *> bList;
    bList<<ui->doubleSpinBox_switch
        <<ui->doubleSpinBox_index
       <<ui->doubleSpinBox_offset;
    if (bList.count() <= d->m_treeWidget->topLevelItemCount()) {
        for (int i = 0; i < bList.count(); i++) {
            d->m_mapping->insertBox2Item(bList.at(i), d->m_treeWidget->topLevelItem(i));
            d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(i), bList.at(i));
        }
    }
}
