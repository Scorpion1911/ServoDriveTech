﻿#include "sevpwrboard.h"
#include "gtutils.h"
#include "qttreemanager.h"
#include "sevdeviceprivate_p.h"
#include "ipwrboard.h"

#include <QDebug>
#include <QTreeWidget>

SevPwrBoard::SevPwrBoard(SevDevicePrivate *sev, QObject *parent):IPwrBoard(sev,parent)
{
  bool isOk;

  QString file=GTUtils::databasePath()+POWERBOAD_NAME;
  QTreeWidget *pwrTree=QtTreeManager::createTreeWidgetFromXmlFile(file);
  //待改进，加一个dsp版本相关，去找控制变量
  PowerTreeManage * pwrTreeManage=new PowerTreeManage(pwrTree);
  pwrTreeManage->updatePowerLimitMapList(sev->m_pwrId,m_powerLimitMapList);
  m_samplingDataInfo=pwrTreeManage->samplingDataInfo(sev->m_pwrId,&isOk);
  delete pwrTreeManage;
  delete pwrTree;
}

SevPwrBoard::~SevPwrBoard()
{
  qDebug()<<"SevPwrBoard-->destruct";
}
QList<QMap<QString ,PowerBoardLimit>>* SevPwrBoard::pwrLimitMapList()
{
  return &m_powerLimitMapList;
}

SamplingDataInfo SevPwrBoard::pwrSamplingDataInfo()
{
  return m_samplingDataInfo;
}
