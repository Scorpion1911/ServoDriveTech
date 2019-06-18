#ifndef VELAUTOTURNING133_H
#define VELAUTOTURNING133_H

#include <QObject>
#include "sdtglobaldef.h"

class SevDevice;
class QTimer;

class  VelAutoTurning133 : public QObject
{
    Q_OBJECT
public:
    explicit VelAutoTurning133(SevDevice *sev,quint16 axisInx,QObject *parent = 0);
    ~VelAutoTurning133();

    qreal velAmp();
    void setVelAmp(qreal value);

    bool isJm();
    void setJmStatus(bool en);

    quint32 getJm();
    void setJm(quint32 value);

    qreal cycleTime();
    void setCycleTime(qreal time);

    bool start();
    void stop();

    quint16 waitMaxCount() const;
    void setWaitMaxCount(quint16 waitMaxCount);

    quint64 autoTnFn();
private:
    void recoverStatus();
    void readAutoTurningResultBeforeServoOff();
signals:
    void progressChanged(int v);
    void autoTurnigFinish(bool ok);
public slots:
    void onTimeOut();
    void onSevDeviceNetError(quint16 axisInx);
private:
    SevDevice *m_sev;
    quint16 m_axisInx;
    qreal m_maxSpd;
    QTimer *m_timer;
    quint32 m_waitCount;
    quint32 m_waitMaxCount;
    qint8 m_progressValue;
    GT::SevTurningMode m_prevTurnMode;
    GT::ModeCtlType m_prevMode;
    GT::SevControlSrc m_prevSrc;
    bool m_isJm;
    qreal m_cycleTime;

    quint64 m_retFn;
    quint32 m_jm;
};

#endif // VELAUTOTURNING133_H
