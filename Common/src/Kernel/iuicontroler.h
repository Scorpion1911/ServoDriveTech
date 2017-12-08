﻿#ifndef IUICONTROLER_H
#define IUICONTROLER_H

#include <QObject>
class GlobalConfig;

class SevDevice;
class IUiControler : public QObject
{
  Q_OBJECT
public:
  explicit IUiControler(SevDevice *sev,GlobalConfig *gconfig,QObject *parent=0);
  virtual ~IUiControler();
  SevDevice *m_sev;
  GlobalConfig *m_gConfig;

signals:

public slots:
};

#endif // IUICONTROLER_H
