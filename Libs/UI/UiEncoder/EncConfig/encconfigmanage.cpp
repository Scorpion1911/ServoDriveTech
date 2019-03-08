#include "encconfigmanage.h"
#include "iencconfigitem.h"
#include "sdtglobaldef.h"
#include  <QDebug>

EncConfigManage::EncConfigManage(QObject *parent) : QObject(parent),mp_curAttributeWidget(NULL)
{

}
EncConfigManage::~EncConfigManage()
{
    for (int i = 0; i < m_encItemList.count(); i++) {
        QList<IEncConfigItem*> itemList = m_encItemList.at(i);
        GT::deepClearList(itemList);
    }
    m_encItemList.clear();
}

void EncConfigManage::creatItemLists(int num)
{
    for (int i = 0; i < num; i++) {
        QList<IEncConfigItem*> list;
        m_encItemList.append(list);
    }
}

void EncConfigManage::addEncItem(int index, IEncConfigItem *encItem)
{
    QList<IEncConfigItem*> list = m_encItemList.at(index);
    list.append(encItem);
    m_encItemList.replace(index, list);
}
IEncConfigItem *EncConfigManage::encItem(int typeInx, quint8 index)
{
    if (typeInx < m_encItemList.count()) {
        if (index < m_encItemList.at(typeInx).count()) {
            return m_encItemList.at(typeInx).at(index);
        }
    }
    return NULL;
}

QList<QStringList> EncConfigManage::itemNames()
{
  QList<QStringList> list;
  for (int i = 0; i < m_encItemList.count(); i++) {
      QStringList itemList;
      for (int j = 0; j < m_encItemList.at(i).count(); j++) {
          IEncConfigItem* item = m_encItemList.at(i).at(j);
          itemList.append(item->objectName());
      }
      list.append(itemList);
  }
  return list;
}

void EncConfigManage::clearAllEncItem()
{
  mp_curAttributeWidget=NULL;
  for (int i = 0; i < m_encItemList.count(); i++) {
      QList<IEncConfigItem*> itemList = m_encItemList.at(i);
      GT::deepClearList(itemList);
  }
  m_encItemList.clear();
}

QWidget *EncConfigManage::curAttributeWidget() const
{
  return mp_curAttributeWidget;
}

void EncConfigManage::setCurAttributeWidget(QWidget *w)
{
  mp_curAttributeWidget = w;
}
