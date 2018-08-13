#include "encconfigbinding130.h"
#include "iencconfigitem.h"

#include <QDebug>
#include <QListWidget>

EncConfigBinding130::EncConfigBinding130(QObject *parent) : IDataBinding(parent)
{

}
EncConfigBinding130::~EncConfigBinding130()
{

}
void EncConfigBinding130::syncMultiUiDataToTree()
{
  IEncConfigItem *encItem=dynamic_cast<IEncConfigItem *>(m_dataMultiObj);
  double gain,value;
  QTreeWidgetItem *item;

  //0 coder_typ_3
  item=m_tree->topLevelItem(0);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=(int)encItem->encType()*gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //2 line_num_3
  item=m_tree->topLevelItem(2);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=encItem->lineNumber()*gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));

  //3 ABS_ENC_CFG.all
  item=m_tree->topLevelItem(3);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=encItem->encConfigData()*gain;
  item->setText(GT::COL_PAGE_TREE_VALUE,QString::number(value,'f',3));
  qDebug()<<"syncMultiUiDataToTree";

}

void EncConfigBinding130::syncMultiTreeToUiData()
{
  IEncConfigItem *encItem=dynamic_cast<IEncConfigItem *>(m_dataMultiObj);
  double gain,value;
  QTreeWidgetItem *item;
  //0 coder_typ_3
  item=m_tree->topLevelItem(0);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  int type=(int)value;
  encItem->setEncType((IEncConfigItem::EncType) type);

  //2 line_num_3
  item=m_tree->topLevelItem(2);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setLineNumber((quint32)value);

  //3 ABS_ENC_CFG.all
  item=m_tree->topLevelItem(3);
  gain=item->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  value=item->text(GT::COL_PAGE_TREE_VALUE).toDouble()/gain;
  encItem->setEncConfigData((quint16)value);
}

void EncConfigBinding130::syncUiDataToTreeItem()
{

}

//!
//! \brief EncConfigBinding130::syncTreeItemToUiData 为了获得当前编码器配置是哪一种
//!
void EncConfigBinding130::syncTreeItemToUiData()
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
  qDebug()<<"TEST_OUT 3";
}

