#include "graphhome136.h"
#include "ui_graphhome136.h"

#include "igraphhome_p.h"
#include "iuiwidget.h"
#include "uihome.h"
#include "sevdevice.h"
#include "homeconfigbinding139.h"
#include "HomeConfig"

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
    Q_D(GraphHome136);
    GT::deletePtrObject(d->m_homeConfigManage);
    delete ui;
}

void GraphHome136::syncTreeDataToUiFace()
{
    Q_D(GraphHome136);
    d->m_mapping->syncAllItem2BoxText();
    initCurrentHomeItem();
    d->m_iDataBinding->multiBind(static_cast<QObject*>(d->m_curHomeConfigItem),d->m_treeWidget);
    d->m_iDataBinding->syncMultiTreeToUiData();
    updateUiByCurrentItem();
}

void GraphHome136::onBtnHomeConfigSaveClicked()
{
    Q_D(GraphHome136);
    if (d->m_curHomeConfigItem != NULL) {
        int index = ui->comboBox_homeType->currentIndex();
        d->m_curHomeConfigItem->setHomeType(index);
        bool reverse = ui->checkBox_homeReverse->isChecked();
        d->m_curHomeConfigItem->setReverse(reverse);
        d->m_curHomeConfigItem->execute();
        d->m_iDataBinding->multiBind(static_cast<QObject*>(d->m_curHomeConfigItem), d->m_treeWidget);
        d->m_iDataBinding->syncMultiUiDataToTree();
    }
}

void GraphHome136::setCustomVisitActive(IUiWidget *uiWidget)
{
    Q_D(GraphHome136);
    Q_UNUSED(uiWidget);
    d->m_iDataBinding = new HomeConfigBinding139(this);
    createHomeItems();
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

void GraphHome136::createHomeItems()
{
    Q_D(GraphHome136);
    d->m_homeConfigManage = new HomeConfigManager;

    IHomeConfigItem *item0 = new HomeItem0(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item0);

    ui->comboBox_homeType->addItems(d->m_homeConfigManage->itemNames());
}

void GraphHome136::initCurrentHomeItem()
{
    Q_D(GraphHome136);
    int index = d->m_treeWidget->topLevelItem(4)->text(GT::COL_PAGE_TREE_VALUE).toInt();
    if (index < ui->comboBox_homeType->count()) {
        d->m_curHomeConfigItem = d->m_homeConfigManage->homeItem(index);
    } else {
        d->m_curHomeConfigItem = NULL;
    }
}

void GraphHome136::updateUiByCurrentItem()
{
    Q_D(GraphHome136);
    if (d->m_curHomeConfigItem != NULL) {
        ui->comboBox_homeType->setCurrentIndex(d->m_curHomeConfigItem->getHomeType());
        ui->checkBox_homeReverse->setChecked(d->m_curHomeConfigItem->getReverse());
    }
}
