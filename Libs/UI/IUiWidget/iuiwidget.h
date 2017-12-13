﻿#ifndef IUIWIDGET_H
#define IUIWIDGET_H

#include "iuiwidget_global.h"
#include <QWidget>
class IUiWidgetPrivate;
class QTreeWidget;
class QStackedWidget;
class QVBoxLayout;
class SevDevice;
class IUIWIDGETSHARED_EXPORT IUiWidget:public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(IUiWidget)
public:
  explicit IUiWidget(QWidget *parent = 0);
  virtual ~IUiWidget();

  virtual bool init(SevDevice*device);
  virtual void addTreeWidget(QTreeWidget* tree);
  void setUiIndexs(quint8 axisInx,quint8 pageInx);
  virtual void readFLASH();
  virtual void writeFLASH();
  virtual void setUiActive(bool actived);

protected:
  virtual QStackedWidget *getUiStackedWidget(void)=0;
  virtual QVBoxLayout *getVBoxLayout(void)=0;
  virtual void setDefaultUi()=0;
  virtual void setCurrentUiIndex(quint8 index);//设置当前两页中显示的页
signals:
  void sglQmlUpdataUi();
  void sglMainErrorInfo(int axis,QString msg);

  //to device
  void sglReadPageFlash(int axis ,QTreeWidget *pTree);
protected:
  IUiWidget(IUiWidgetPrivate&d, QWidget *parent=0);
  IUiWidgetPrivate *d_ptr;
};

#endif // IUIWIDGET_H