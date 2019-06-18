#include "velautoturning133.h"
#include "sevdevice.h"
#include "gtutils.h"

#include <QTimer>
#include <QDebug>

#define CMD_AUTO_TURNING_MODE "gSevDrv.sev_obj.vel.atn.prm.cfg_flag.all"  //118
#define CMD_SPD_AMP           "gSevDrv.sev_obj.vel.atn.prm.spd_cmd_amp"         //119
#define CMD_CYCLE_TIME        "gSevDrv.sev_obj.vel.atn.prm.idf_mov_tim"   //120
#define CMD_FIRST_FN          "gSevDrv.sev_obj.vel.ctl.prm.fn_fst_5"
#define CMD_JM_FINISH_FLAG    "gSevDrv.sev_obj.vel.atn.jm_idf_finish_flag" //121
#define CMD_JM_VALUE          "gSevDrv.sev_obj.cur.mot.Jm_1"

VelAutoTurning133::VelAutoTurning133(SevDevice *sev, quint16 axisInx, QObject *parent) : QObject(parent),
    m_sev(sev),
    m_axisInx(axisInx),
    m_maxSpd(0),
    m_waitCount(0),
    m_waitMaxCount(2000000),
    m_progressValue(0),
    m_retFn(0),
    m_isJm(false),
    m_cycleTime(0),
    m_jm(0)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(100);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    connect(m_sev, SIGNAL(netError(quint16)), this, SLOT(onSevDeviceNetError(quint16)));
}

VelAutoTurning133::~VelAutoTurning133()
{

}

qreal VelAutoTurning133::velAmp()
{
    return m_maxSpd;
}

void VelAutoTurning133::setVelAmp(qreal value)
{
    m_maxSpd = value;
}

bool VelAutoTurning133::isJm()
{
    return m_isJm;
}

void VelAutoTurning133::setJmStatus(bool en)
{
    m_isJm = en;
}

quint32 VelAutoTurning133::getJm()
{
    return m_jm;
}

void VelAutoTurning133::setJm(quint32 value)
{
    m_jm = value;
}

qreal VelAutoTurning133::cycleTime()
{
    return m_cycleTime;
}

void VelAutoTurning133::setCycleTime(qreal time)
{
    m_cycleTime = time;
}

bool VelAutoTurning133::start()
{
    m_prevMode = (GT::ModeCtlType)m_sev->currentTaskServoMode(m_axisInx);
    m_prevSrc = m_sev->controlSrc(m_axisInx);
    bool ok;
    m_prevTurnMode = (GT::SevTurningMode)m_sev->genCmdRead(CMD_AUTO_TURNING_MODE, m_axisInx, ok);

      //获取控制权
    bool getControl = false;
    for (int i = 0; i < 100; i++) {
        m_sev->setControlSrc(m_axisInx,GT::SEV_CTL_SRC_PC);
        GTUtils::delayms(10);
        if(GT::SEV_CTL_SRC_PC == m_sev->controlSrc(m_axisInx))
        {
          getControl = true;
          break;
        }
    }
    if (!getControl) {
        recoverStatus();
        return false;
    }

      //设置AUT模式
    bool modeOk = false;
    for (int i = 0; i < 100;i++) {
        m_sev->setCurrentTaskServoMode(m_axisInx, GT::MODE_AUT);
        GTUtils::delayms(10);
        if (GT::MODE_AUT == m_sev->currentTaskServoMode(m_axisInx)) {
            modeOk = true;
            break;
        }
    }
    if (!modeOk) {
        recoverStatus();
        return false;
    }

    bool turningOk = false;
    for (int i = 0; i < 100;i++) {
        if (!m_isJm) {
            m_sev->genCmdWrite(CMD_AUTO_TURNING_MODE, GT::SEV_TURNING_VELOCITY, m_axisInx);
            GTUtils::delayms(10);
            if (GT::SEV_TURNING_VELOCITY == m_sev->genCmdRead(CMD_AUTO_TURNING_MODE, m_axisInx, ok)) {
                turningOk = true;
                break;
            }
        } else {
            m_sev->genCmdWrite(CMD_AUTO_TURNING_MODE, GT::SEV_TURNING_VEL_JM, m_axisInx);
            GTUtils::delayms(10);
            if (GT::SEV_TURNING_VEL_JM == m_sev->genCmdRead(CMD_AUTO_TURNING_MODE, m_axisInx, ok)) {
                turningOk = true;
                break;
            }
        }
    }
    if (!turningOk) {
        recoverStatus();
        return false;
    }

    ok = m_sev->genCmdWrite(CMD_SPD_AMP, m_maxSpd, m_axisInx);
    if (!ok) {
        ok = m_sev->genCmdWrite(CMD_SPD_AMP, m_maxSpd, m_axisInx);
        if (!ok) {
            return false;
        }
    }

    ok = m_sev->genCmdWrite(CMD_CYCLE_TIME, m_cycleTime, m_axisInx);
    if (!ok) {
        ok = m_sev->genCmdWrite(CMD_CYCLE_TIME, m_cycleTime, m_axisInx);
        if (!ok) {
            return false;
        }
    }

      //上伺服
    bool servoIsOn = false;
    for (int count = 0; count < 100; count++) {
        m_sev->setAxisServoOn(m_axisInx,true);
        GTUtils::delayms(10);
        servoIsOn = m_sev->axisServoIsOn(m_axisInx);
        if (servoIsOn)
        {
          break;
        }

    }

    if (!servoIsOn) {
        recoverStatus();
        return false;
    }

    m_timer->start();

    return true;
}

void VelAutoTurning133::stop()
{
    m_progressValue = 0;
    m_waitCount = 0;
    m_timer->stop();
    readAutoTurningResultBeforeServoOff();
    recoverStatus();
}

quint16 VelAutoTurning133::waitMaxCount() const
{
    return m_waitMaxCount;
}

void VelAutoTurning133::setWaitMaxCount(quint16 waitMaxCount)
{
    m_waitMaxCount = waitMaxCount;
}

quint64 VelAutoTurning133::autoTnFn()
{
    return m_retFn;
}

void VelAutoTurning133::recoverStatus()
{
    if (m_sev->isConnecting()) {
        m_sev->setAxisServoOn(m_axisInx,false);
        m_sev->setAxisServoOn(m_axisInx,false);
        m_sev->setControlSrc(m_axisInx,m_prevSrc);
        m_sev->setCurrentTaskServoMode(m_axisInx, m_prevMode);
        m_sev->genCmdWrite(CMD_AUTO_TURNING_MODE, m_prevTurnMode, m_axisInx);
    }
}

void VelAutoTurning133::readAutoTurningResultBeforeServoOff()
{
    bool isOk = false;
    m_retFn = m_sev->genCmdRead(CMD_FIRST_FN, m_axisInx, isOk);
    if (m_isJm) {
        m_jm = m_sev->genCmdRead(CMD_JM_VALUE, m_axisInx, isOk);
    }

}

void VelAutoTurning133::onTimeOut()
{
    m_progressValue ++;
    m_waitCount ++;
    emit progressChanged(m_progressValue % 100);

    bool isOk = false;
    bool finish = false;
    bool jmFinish = false;
    if (!m_isJm) {
        finish = m_sev->genCmdAutoTurnningFinishFlag(m_axisInx, isOk);
        if (finish) {
            stop();
            emit autoTurnigFinish(true);
        }
        if (m_waitCount > m_waitMaxCount) {
            stop();
            emit autoTurnigFinish(false);
        }
    } else {
        finish = m_sev->genCmdAutoTurnningFinishFlag(m_axisInx, isOk);
        jmFinish = m_sev->genCmdRead(CMD_CYCLE_TIME, m_axisInx, isOk);
        if (finish & jmFinish) {
            stop();
            emit autoTurnigFinish(true);
        }
        if (m_waitCount > m_waitMaxCount) {
            stop();
            emit autoTurnigFinish(false);
        }
    }
}

void VelAutoTurning133::onSevDeviceNetError(quint16 axisInx)
{
    if (axisInx ==  m_axisInx) {
        m_progressValue = 0;
        m_waitCount = 0;
        m_timer->stop();
    }
}

