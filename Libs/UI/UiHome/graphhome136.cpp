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

    IHomeConfigItem *item1 = new HomeItem1(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item1);

    IHomeConfigItem *item2 = new HomeItem2(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item2);

    IHomeConfigItem *item3 = new HomeItem3(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item3);

    IHomeConfigItem *item4 = new HomeItem4(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item4);

    IHomeConfigItem *item5 = new HomeItem5(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item5);

    IHomeConfigItem *item6 = new HomeItem6(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item6);

    IHomeConfigItem *item7 = new HomeItem7(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item7);

    IHomeConfigItem *item8 = new HomeItem8(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item8);

    IHomeConfigItem *item9 = new HomeItem9(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item9);

    IHomeConfigItem *item10 = new HomeItem10(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item10);

    IHomeConfigItem *item11 = new HomeItem11(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item11);

    IHomeConfigItem *item12 = new HomeItem12(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item12);

    IHomeConfigItem *item13 = new HomeItem13(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item13);

    IHomeConfigItem *item14 = new HomeItem14(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item14);

    IHomeConfigItem *item15 = new HomeItem15(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item15);

    IHomeConfigItem *item16 = new HomeItem16(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item16);

    IHomeConfigItem *item17 = new HomeItem17(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item17);

    IHomeConfigItem *item18 = new HomeItem18(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item18);

    IHomeConfigItem *item19 = new HomeItem19(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item19);

    IHomeConfigItem *item20 = new HomeItem20(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item20);

    IHomeConfigItem *item21 = new HomeItem21(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item21);

    IHomeConfigItem *item22 = new HomeItem22(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item22);

    IHomeConfigItem *item23 = new HomeItem23(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item23);

    IHomeConfigItem *item24 = new HomeItem24(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item24);

    IHomeConfigItem *item25 = new HomeItem25(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item25);

    IHomeConfigItem *item26 = new HomeItem26(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item26);

    IHomeConfigItem *item27 = new HomeItem27(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item27);

    IHomeConfigItem *item28 = new HomeItem28(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item28);

    IHomeConfigItem *item29 = new HomeItem29(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item29);

    IHomeConfigItem *item30 = new HomeItem30(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item30);

    IHomeConfigItem *item31 = new HomeItem31(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item31);

    IHomeConfigItem *item32 = new HomeItem32(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item32);

    IHomeConfigItem *item33 = new HomeItem33(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item33);

    IHomeConfigItem *item34 = new HomeItem34(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item34);

    IHomeConfigItem *item35 = new HomeItem35(d->m_uiWidget->uiIndexs().axisInx, d->m_dev);
    d->m_homeConfigManage->addHomeItem(item35);

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
