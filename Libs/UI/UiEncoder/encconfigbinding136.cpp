#include "encconfigbinding136.h"
#include "iencconfigitem.h"

#include <QDebug>
#include <QListWidget>

EncConfigBinding136::EncConfigBinding136(QObject *parent) : IDataBinding(parent)
{

}
EncConfigBinding136::~EncConfigBinding136()
{

}
void EncConfigBinding136::syncMultiUiDataToTree()
{
  IEncConfigItem *encItem=dynamic_cast<IEncConfigItem *>(m_dataMultiObj);
  double gain, value;
  QTreeWidgetItem *item;

  qDebug()<<"name"<<encItem->objectName();

  //0 coder_typ_3
//  item=m_tree->topLevelItem(0);
//  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
//  value=(int)encItem->encType()*gain;
//  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //2 line_num_3
  item=m_tree->topLevelItem(2);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=encItem->lineNumber()*gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //3 ABS_ENC_CFG.all
  item=m_tree->topLevelItem(3);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->encConfigData() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //5 COMBO_TYPE
  item = m_tree->topLevelItem(5);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->comboType() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  // 6 7 abs lineNum
  QTreeWidgetItem* itemLow = m_tree->topLevelItem(6);
  double gainLow = itemLow->text(GT::COL_PAGE_TREE_SCALE).toDouble();

  QTreeWidgetItem* itemHigh = m_tree->topLevelItem(7);
  double gainHigh = itemHigh->text(GT::COL_PAGE_TREE_SCALE).toDouble();

  quint32 absValue = encItem->absLineNum();

  double valueLow = (absValue & 0xFFFF) * gainLow;
  double valueHigh = ((absValue >> 16) & 0xFFFF) * gainHigh;

  itemLow->setText(GT::COL_PAGE_TREE_VALUE,QString::number(valueLow,'f',3));
  itemHigh->setText(GT::COL_PAGE_TREE_VALUE,QString::number(valueHigh,'f',3));

  //8 adGain
  item = m_tree->topLevelItem(8);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->adGain() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //9 sinCfg
  item = m_tree->topLevelItem(9);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->sinCfg() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //12 13 pulse z count
  itemLow = m_tree->topLevelItem(12);
  gainLow = itemLow->text(GT::COL_PAGE_TREE_SCALE).toDouble();

  itemHigh = m_tree->topLevelItem(13);
  gainHigh = itemHigh->text(GT::COL_PAGE_TREE_SCALE).toDouble();

  quint32 zCountValue = encItem->pulseZCount();

  valueLow = (zCountValue & 0xFFFF) * gainLow;
  valueHigh = ((zCountValue >> 16) & 0xFFFF) * gainHigh;

  itemLow->setText(GT::COL_PAGE_TREE_VALUE,QString::number(valueLow,'f',3));
  itemHigh->setText(GT::COL_PAGE_TREE_VALUE,QString::number(valueHigh,'f',3));

  //14 shift num
  item = m_tree->topLevelItem(14);
  gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->shiftNum() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //15 abmodel
  item = m_tree->topLevelItem(15);
  gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->pulseABModel() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //16 auxCfg
  item = m_tree->topLevelItem(16);
  gain = item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value = encItem->aufCfg() * gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  qDebug()<<"syncMultiUiDataToTree";

}

void EncConfigBinding136::syncMultiTreeToUiData()
{
  IEncConfigItem *encItem=dynamic_cast<IEncConfigItem *>(m_dataMultiObj);
  double gain,value;
  QTreeWidgetItem *item;
//  //0 coder_typ_3
//  item=m_tree->topLevelItem(0);
//  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
//  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
//  int type=(int)value;
//  encItem->setEncType((IEncConfigItem::EncType) type);

  //2 line_num_3
  item=m_tree->topLevelItem(2);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  qDebug()<<"line value"<<value;
  encItem->setLineNumber((quint32)value);

  //3 ABS_ENC_CFG.all
  item=m_tree->topLevelItem(3);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setEncConfigData((quint16)value);

  //5 COMBO_TYPE
  item = m_tree->topLevelItem(5);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setComboType((quint16)value);

  // 6 7 abs lineNum
  QTreeWidgetItem* itemLow = m_tree->topLevelItem(6);
  double gainLow = itemLow->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  double valueLow = itemLow->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gainLow;

  QTreeWidgetItem* itemHigh = m_tree->topLevelItem(7);
  double gainHigh = itemHigh->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  double valueHigh = itemHigh->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gainHigh;

  value = (quint16)valueLow + ((quint16)valueHigh << 16);

  encItem->setAbsLineNum((quint32)value);

  //8 adGain
  item = m_tree->topLevelItem(8);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setAdGain((quint16)value);

  //9 sinCfg
  item = m_tree->topLevelItem(9);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setSinCfg((quint16)value);

  //12 13 pulse z count
  itemLow = m_tree->topLevelItem(12);
  gainLow = itemLow->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  valueLow = itemLow->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gainLow;

  itemHigh = m_tree->topLevelItem(13);
  gainHigh = itemHigh->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  valueHigh = itemHigh->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gainHigh;

  value = (quint16)valueLow + ((quint16)valueHigh << 16);

  encItem->setPulseZCount((quint32)value);

  //14 shift num
  item = m_tree->topLevelItem(14);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setShiftNum((quint16)value);

  // 15 pulse ab model
  item = m_tree->topLevelItem(15);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setPulseABModel((quint16)value);

  //16 aux cfg
  item = m_tree->topLevelItem(16);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setAufCfg((quint16)value);
}

void EncConfigBinding136::syncUiDataToTreeItem()
{

}

//!
//! \brief EncConfigBinding136::syncTreeItemToUiData 为了获得当前编码器配置是哪一种
//!
void EncConfigBinding136::syncTreeItemToUiData()
{
  qDebug()<<"TEST_OUT 1";
  QListWidget *listWidget=dynamic_cast<QListWidget *>(m_dataObj);
  qDebug()<<"text value"<<m_treeItem->text(GT::COL_PAGE_TREE_VALUE);
  quint16 index=m_treeItem->text(GT::COL_PAGE_TREE_VALUE).toDouble();
  index&=0x000f;//V130版本中0-3bit代表编码器类型 12-15是波特率
  qDebug()<<"TEST_OUT 2 index="<<index;
  if(index<listWidget->count())
    listWidget->setCurrentRow(index);
  else
    listWidget->setCurrentRow(0);
  qDebug()<<"cur row"<<listWidget->currentRow();
  qDebug()<<"TEST_OUT 3";
}

