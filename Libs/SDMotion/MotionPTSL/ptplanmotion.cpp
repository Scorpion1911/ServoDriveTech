#include "ptplanmotion.h"
#include "uimotionptsl.h"
#include "sevdevice.h"
#include "gtutils.h"
#include <QDebug>

#define PT_TIME_INTERVAL 10
#define PT_INIT_POINT 150
#define PT_MIN_POINT 100
#define PT_ADD_POINT 30
#define POW2_24 16777216

#define PT_BUFFER_DATA_IN       "gSevDrv.sev_obj.pos.ptip.buffer"
#define PT_MOTION_EN            "gSevDrv.sev_obj.pos.ptip.flag.bit.EN"
#define PT_BUFFER_DATA_NUM      "gSevDrv.sev_obj.pos.ptip.res_count"
#define PT_BUFFER_DATA_CLR      "gSevDrv.sev_obj.pos.ptip.flag.bit.EMPTY"

PTPlanMotion::PTPlanMotion(quint16 axisInx, SevDevice *dev, UiPTSLMotionData *data, QObject *parent) : QObject(parent),
    m_sev(dev),
    m_data(data),
    m_axisInx(axisInx)
{
    m_timer.setTimerType(Qt::PreciseTimer);
    m_timer.setInterval(200);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

PTPlanMotion::~PTPlanMotion()
{

}

QTimer *PTPlanMotion::timer()
{
    return &m_timer;
}

void PTPlanMotion::movePrepare()
{
    m_timeVector.clear();
    m_posVector.clear();
    m_count = 0;
    m_data->m_motionSta = UiPTSLMotionData::RUN_STA_INIT;
    if (m_data->m_isReci) {
        double singleTime = (m_data->m_reciAcc + m_data->m_reciDec + m_data->m_reciCons + m_data->m_reciInterval) * 2 / PT_TIME_INTERVAL;
        for (int i = 0; i < m_data->m_reciTimes; i++) {
            double consVel = m_data->m_reciMaxVel * POW2_24 / 60;
            double acc = consVel / m_data->m_reciAcc;
            double dec = consVel / m_data->m_reciDec;
            double t1 = m_data->m_reciAcc / PT_TIME_INTERVAL;
            double t2 = (m_data->m_reciAcc + m_data->m_reciCons) / PT_TIME_INTERVAL;
            double t3 = (m_data->m_reciAcc + m_data->m_reciCons + m_data->m_reciDec) / PT_TIME_INTERVAL;
            double t4 = (m_data->m_reciAcc + m_data->m_reciDec + m_data->m_reciCons + m_data->m_reciInterval) / PT_TIME_INTERVAL;
            double t5 = (2 * m_data->m_reciAcc + m_data->m_reciDec + m_data->m_reciCons + m_data->m_reciInterval) / PT_TIME_INTERVAL;
            double t6 = (2 * m_data->m_reciAcc + m_data->m_reciDec + 2 * m_data->m_reciCons + m_data->m_reciInterval) / PT_TIME_INTERVAL;
            double t7 = (2 * m_data->m_reciAcc + 2 * m_data->m_reciDec + 2 * m_data->m_reciCons + m_data->m_reciInterval) / PT_TIME_INTERVAL;
            for (int j = 1; j <= singleTime; j++) {
                if (j <= t1) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(j * acc);
                } else if (j <= t2) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(consVel);
                } else if (j <= t3) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(consVel - dec * (j - t2));
                } else if (j <= t4) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(0);
                } else if (j <= t5) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(-acc * (j - t4));
                } else if (j <= t6) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(-consVel);
                } else if (j <= t7) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(-consVel + dec * (j - t6));
                } else {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_reciTimes);
                    m_posVector.append(0);
                }
            }
        }
    } else {
        double singleTime = (m_data->m_pointAcc + m_data->m_pointDec + m_data->m_pointCons + 500) / PT_TIME_INTERVAL;
        for (int i = 0; i < m_data->m_pointTimes; i++) {
            double consVel = m_data->m_pointMaxVel * POW2_24 / 60;
            double acc = consVel / m_data->m_pointAcc;
            double dec = consVel / m_data->m_pointDec;
            double t1 = m_data->m_pointAcc / PT_TIME_INTERVAL;
            double t2 = (m_data->m_pointAcc + m_data->m_pointCons) / PT_TIME_INTERVAL;
            double t3 = (m_data->m_pointAcc + m_data->m_pointCons + m_data->m_pointDec) / PT_TIME_INTERVAL;
            for (int j = 1; j <= singleTime; j++) {
                if (j <= t1) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_pointTimes);
                    m_posVector.append(j * acc);
                } else if (j <= t2) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_pointTimes);
                    m_posVector.append(consVel);
                } else if (j <= t3) {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_pointTimes);
                    m_posVector.append(consVel - dec * (j - t2));
                } else {
                    m_timeVector.append(j * PT_TIME_INTERVAL * m_data->m_pointTimes);
                    m_posVector.append(0);
                }
            }
        }
    }
    m_length = m_timeVector.count();
    m_sev->genCmdWrite(PT_BUFFER_DATA_CLR, 0, m_axisInx);
    for (int i = 0; i < PT_INIT_POINT; i++) {
        if (i >= m_length) {
            break;
        }
        writeData(i);
    }
}

void PTPlanMotion::move()
{
    m_sev->genCmdWrite(PT_MOTION_EN, 1, m_axisInx);
    m_timer.start();
}

void PTPlanMotion::stop()
{
    m_sev->genCmdWrite(PT_MOTION_EN, 0, m_axisInx);
    m_timer.stop();
    m_sev->genCmdWrite(PT_BUFFER_DATA_CLR, 0, m_axisInx);
    m_posVector.clear();
    m_timeVector.clear();
    emit motionFinish(m_axisInx);
}

bool PTPlanMotion::writeData(int index)
{
    qint64 pos = (qint32)m_posVector.at(index);
    qint64 time = (qint32)m_timeVector.at(index);
    qint64 data = pos + time<<32;
    bool ok = m_sev->genCmdWrite(PT_BUFFER_DATA_IN, data, m_axisInx);
    if (ok) {
        m_count++;
    }
    return ok;
}

void PTPlanMotion::onTimerOut()
{
    bool ok;
    quint16 pointNum = m_sev->genCmdRead(PT_BUFFER_DATA_NUM, m_axisInx, ok);
    if (m_count >= m_length && pointNum == 0) {
        stop();
        return;
    }
    emit progressValueChanged(m_axisInx, m_count * 100 / m_length);
    if (pointNum < PT_MIN_POINT) {
        for (int i = 0; i < PT_ADD_POINT; i++) {
            if (i >= m_length) {
                break;
            }
            writeData(i);
        }
    }
}

