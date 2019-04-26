#include "homeconfigbinding139.h"
#include "ihomeconfigitem.h"

#include <QComboBox>
#include <QDebug>

HomeConfigBinding139::HomeConfigBinding139(QObject *parent) : IDataBinding(parent)
{

}

HomeConfigBinding139::~HomeConfigBinding139()
{
    
}

void HomeConfigBinding139::syncMultiUiDataToTree()
{
    IHomeConfigItem *homeItem = dynamic_cast<IHomeConfigItem *>(m_dataMultiObj);
    double gain, value;
    QTreeWidgetItem *item;
    
    qDebug()<<"name"<<homeItem->objectName();
    
    // 3 reverse
    item = m_tree->topLevelItem(3);
    gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
    bool reverse = homeItem->getReverse();
    if (reverse) {
        value = 1 * gain;
    } else {
        value = 0 * gain;
    }
    item->setText(GT::COL_PAGE_TREE_VALUE, QString::number(value,'f',3));
    
    // 4 type
    item = m_tree->topLevelItem(4);
    gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
    value = homeItem->getHomeType() * gain;
    item->setText(GT::COL_PAGE_TREE_VALUE, QString::number(value,'f',3));
    
    // 5 config
    item = m_tree->topLevelItem(5);
    gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
    value = homeItem->getHomeConfig() * gain;
    item->setText(GT::COL_PAGE_TREE_VALUE, QString::number(value,'f',3));
}

void HomeConfigBinding139::syncMultiTreeToUiData()
{
    IHomeConfigItem *homeItem = dynamic_cast<IHomeConfigItem *>(m_dataMultiObj);
    double gain, value;
    QTreeWidgetItem *item;
    
    // 3 reverse
    item = m_tree->topLevelItem(3);
    gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
    value = item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
    homeItem->setReverse(!(value == 0));
    
    // 5 config
    item = m_tree->topLevelItem(5);
    gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
    value = item->text(GT::COL_PAGE_TREE_VALUE).toDouble() / gain;
    homeItem->setHomeConfig(quint16(value));
}

void HomeConfigBinding139::syncUiDataToTreeItem()
{

}

void HomeConfigBinding139::syncTreeItemToUiData()
{
    QComboBox* comboBox = dynamic_cast<QComboBox*>(m_dataObj);
    quint16 index = m_treeItem->text(GT::COL_PAGE_TREE_VALUE).toDouble();
    if (index < comboBox->count())
        comboBox->setCurrentIndex(index);
      else
        comboBox->setCurrentIndex(0);
}

