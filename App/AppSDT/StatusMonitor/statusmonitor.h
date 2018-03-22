﻿#ifndef STATUSMONITOR_H
#define STATUSMONITOR_H

#include <QObject>
class SevDevice;
class QTimer;

class StatusMonitor : public QObject
{
  Q_OBJECT
public:
  explicit StatusMonitor(QObject *parent = 0);
  ~StatusMonitor();
  void setMonitorDeviceList(QList<SevDevice*>devList);
  void startMonitor(quint16 ms);
  void stopMonitor();

signals:
  void netError(quint16 devId);
  void alarmError(quint16 devId,quint16 axisInx,bool hasError);
private slots:
  void onMonitorTimeOut();
  void onAlarmError(quint16 devId,quint16 axisInx,bool hasError);
private:
  QList<SevDevice*>m_sevDeviceList;
  QTimer *m_timer;
};

#endif // STATUSMONITOR_H