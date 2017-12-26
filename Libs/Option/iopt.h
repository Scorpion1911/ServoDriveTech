﻿#ifndef IOPT_H
#define IOPT_H

#include <QWidget>
#include "option_global.h"
class IOptPrivate;
class QSettings;
class OPTIONSHARED_EXPORT IOpt : public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(IOpt)
public:
  explicit IOpt(const QString &optName, QWidget *parent = 0);
  virtual ~IOpt();
  QString name() const;

  bool execute();

  bool saveOptToFile();

protected:
  IOpt(const QString &optName, IOptPrivate&dd, QWidget *parent = 0);
  virtual bool optActive()=0;
  virtual bool readOpt(QSettings *settings)=0;
  virtual bool writeOpt(QSettings *settings)=0;

  bool readOptFile();
  bool isModify() const;
  void setModify(bool modify);

  void saveData(QSettings *settings,const QString &group,const QString &key,const QVariant &value);
  QVariant data(QSettings *settings,const QString &group,const QString &key,const QVariant &defaultValue);

private:


signals:

public slots:

protected:
  IOptPrivate *d_ptr;

};

#endif // IOPT_H