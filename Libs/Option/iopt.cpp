﻿#include "iopt.h"
#include "iopt_p.h"
#include "gtutils.h"

#include <QDebug>
#include <QSettings>

#define OPT_START_INI "opt.ini"

QString IOptPrivate::m_optPath=GTUtils::customPath()+"option/";

IOptPrivate::IOptPrivate():m_name("iopt"),m_isModify(false)
{

}

IOptPrivate:: ~IOptPrivate()
{

}

IOpt::IOpt(const QString &optName, QWidget *parent) : QWidget(parent),d_ptr(new IOptPrivate)
{
  d_ptr->q_ptr=this;
  d_ptr->m_name=optName;
}
IOpt::~IOpt()
{

}

IOpt::IOpt(const QString &optName, IOptPrivate&dd, QWidget *parent):QWidget(parent),d_ptr(&dd)
{
  d_ptr->q_ptr=this;
  d_ptr->m_name=optName;
}

QString IOpt::name() const
{
  Q_D(const IOpt);
  return d->m_name;
}

bool IOpt::isModify() const
{
  Q_D(const IOpt);
  return d->m_isModify;
}
void IOpt::setModify(bool modify)
{
  Q_D(IOpt);
  d->m_isModify=modify;
}
bool IOpt::execute()
{
  Q_D(IOpt);
  bool ok=true;
  if(d->m_isModify)
  {
    ok=optActive();
    qDebug()<<d->m_name<<"execute";
    d->m_isModify=false;
  }
  return ok;
}
bool IOpt::saveOptToFile()
{
  Q_D(IOpt);
  QSettings settings(d->m_optPath+OPT_START_INI,
                     QSettings::IniFormat);
  writeOpt(&settings);
  return true;
}

bool IOpt::readOptFile()
{
  Q_D(IOpt);
  bool ok=true;
  QSettings settings(d->m_optPath+OPT_START_INI,
                     QSettings::IniFormat);
  qDebug()<<"iopt"<<"read opt file";
  ok=readOpt(&settings);
  return ok;
}
void IOpt::saveData(QSettings *settings,const QString &group,const QString &key,const QVariant &value)
{
  settings->beginGroup(group);
  settings->setValue(key, value);
  settings->endGroup();
}

QVariant IOpt::data(QSettings *settings, const QString &group, const QString &key, const QVariant &defaultValue)
{
  QVariant d;
  settings->beginGroup(group);
  d=settings->value(key,defaultValue);
  settings->endGroup();
  return d;
}