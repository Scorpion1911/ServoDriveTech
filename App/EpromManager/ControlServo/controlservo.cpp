#include "controlservo.h"
#include "globaldefine.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QTreeWidgetItemIterator>
#include <QMessageBox>
#define TEST_DEBUG 0



ControlServo::ControlServo(QObject *parent) : QObject(parent)
{

}

ControlServo::~ControlServo()
{

}

//写单个树结点
void ControlServo::writeServoFlash(QTreeWidgetItem *item, int axisIndex, COM_TYPE comtype,short comStation)
{
  Uint16 offsetValue = 0;
  QString itemType;
  int16 result=0;//0代表成功返回

  QString offsetStr = item->text(COL_ADDRESS);
  offsetValue = offsetStr.toUShort();
  if (offsetStr.compare("-1") != 0)
  {
      itemType = item->text(COL_TYPE);
      if(itemType == "Uint16")
      {
        uint16 wv;
        int16 rv;
        double value=0;
        value = item->text(COL_VALUE).toDouble()+0.5;
        wv = (uint16)value;
        quint8 tryCount = 0;
        do {
            result = GTSD_CMD_Fram_Write16BitByAdr(axisIndex, offsetValue, (int16)wv, (int16)comtype, comStation);
            result = GTSD_CMD_Fram_Read16BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
            tryCount++;
        } while (((uint16)rv != wv)&&(tryCount < 2));
        if(tryCount >= 2)
          return;
      }
      else if(itemType=="int16")
      {
        int16 wv;
        int16 rv;
        wv=item->text(COL_VALUE).toShort();

        quint8 tryCount=0;
        do {
          result = GTSD_CMD_Fram_Write16BitByAdr(axisIndex, offsetValue, wv, (int16)comtype, comStation);
          result = GTSD_CMD_Fram_Read16BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
          tryCount++;
        }while((rv!=wv)&&(tryCount<2));
        //qDebug()<<"tryCount"<<tryCount<<"rv ="<<rv<<"wv ="<<wv;
        if(tryCount>=2)
          return;
      }
      else if(itemType=="Uint32")
      {
        uint32 wv;
        int32 rv;
        double value=0;
        value=item->text(COL_VALUE).toDouble()+0.5;
        wv=(uint32)value;
        //wv=item->text(COL_PAGE_TREE_VALUE).toULong();

        quint8 tryCount=0;
        do{
          result = GTSD_CMD_Fram_Write32BitByAdr(axisIndex, offsetValue, (int32)wv, (int16)comtype, comStation);
          result = GTSD_CMD_Fram_Read32BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
          tryCount++;
        }while(((uint32)rv!=wv)&&(tryCount<2));
        if(tryCount>=2)
          return;
      }
      else if(itemType=="int32")
      {
        int32 wv;
        int32 rv;
        //wv=item->text(COL_PAGE_TREE_VALUE).toLong();
        double value=0;
        value=item->text(COL_VALUE).toDouble()+0.5;
        wv=(int32)value;
        quint8 tryCount=0;
        do{
          result = GTSD_CMD_Fram_Write32BitByAdr(axisIndex, offsetValue, wv, (int16)comtype, comStation);
          result = GTSD_CMD_Fram_Read32BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
          tryCount++;
        }while((rv!=wv)&&(tryCount<2));
        if(tryCount>=2)
          return;
      }
      else if(itemType=="Uint64")
      {
        uint64 wv;
        int64 rv;
        double value=0;
        value=item->text(COL_VALUE).toDouble()+0.5;
        wv=(uint64)value;
        //wv=item->text(COL_PAGE_TREE_VALUE).toULongLong();

        quint8 tryCount=0;
        do{
          result = GTSD_CMD_Fram_Write64BitByAdr(axisIndex, offsetValue, (int64)wv, (int16)comtype, comStation);
          result = GTSD_CMD_Fram_Read64BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
          tryCount++;
        }while(((uint64)rv!=wv)&&(tryCount<2));
        if(tryCount>=2)
          return;
      }
      else if(itemType=="int64")
      {
        int64 wv;
        int64 rv;
        double value=0;
        value=item->text(COL_VALUE).toDouble()+0.5;
        wv=(int64)value;
        //wv=item->text(COL_PAGE_TREE_VALUE).toLongLong();

        quint8 tryCount=0;
        do{
          result = GTSD_CMD_Fram_Write64BitByAdr(axisIndex, offsetValue, wv, (int16)comtype, comStation);
          result = GTSD_CMD_Fram_Read64BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
          tryCount++;
        }while((rv!=wv)&&(tryCount<2));
        if(tryCount>=2)
          return;
      }
      else
      {
        uint16 wv;
        int16 rv;
        double value=0;
        value=item->text(COL_VALUE).toDouble()+0.5;
        wv=(uint16)value;
        //wv=item->text(COL_PAGE_TREE_VALUE).toUShort();

        quint8 tryCount=0;
        do{
          result = GTSD_CMD_Fram_Write16BitByAdr(axisIndex, offsetValue, (int16)wv, (int16)comtype, comStation);
          result = GTSD_CMD_Fram_Read16BitByAdr(axisIndex, offsetValue, &rv, (int16)comtype, comStation);
          tryCount++;
        }while(((uint16)rv!=wv)&&(tryCount<2));
        if(tryCount>=2)
          return;
      }
  }
}

void ControlServo::writeServoFlashByAxisTree(QTreeWidget *tree, int axisIndex, COM_TYPE comtype,short comStation)
{
  QTreeWidgetItemIterator it(tree);
  QTreeWidgetItem *item;
  quint32 count=totalItemCount(tree);
  quint32 index=0;
  qDebug()<<"totalcount="<<count;
  double value;
  while (*it)
  {
    item=(*it);
    index++;
    writeServoFlash(item, axisIndex, comtype,comStation);
    it++;
    value=((index+0.0)/(count+0.0))*100;
//    qDebug()<<"index="<<index<<"value="<<value;
    emit progressValue((int)value);
  }
}

void ControlServo::writeServoFlashByAllAxisTree(QTreeWidget *alltree, COM_TYPE comtype,short comStation)
{
  QTreeWidget *tree;
  QTreeWidgetItem *treeItem;
  for(int i=0;i<alltree->topLevelItemCount();i++)
  {
    tree=new QTreeWidget();
    treeItem=alltree->topLevelItem(i)->clone();
    tree->addTopLevelItem(treeItem);
    writeServoFlashByAxisTree(tree,i,comtype,comStation);
    tree->clear();
    delete tree;
    tree=NULL;
  }
}

quint32 ControlServo::totalItemCount(QTreeWidget*tree)
{
  QTreeWidgetItemIterator it(tree);
  quint32 count=0;
  while (*it)
  {
    count++;
    it++;
  }
  return count;
}
