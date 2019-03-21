#ifndef UIMOTIONPTSL_H
#define UIMOTIONPTSL_H

#include "motionptsl_global.h"

#include <QWidget>

namespace Ui {
class UiMotionPTSL;
}

class MotionPTSL;
class SevDevice;

class UiPTSLMotionData
{

public:
    typedef enum{
        RUN_STA_INIT = 0,
        RUN_STA_CHECK_ON = 1,
        RUN_STA_RUNNING = 2,
        RUN_STA_STOPING = 3,
        RUN_STA_STOP_FINISH = 4
    }MotionRunSta;
    UiPTSLMotionData():
        m_isReci(false),
        m_pointAcc(2000),
        m_pointDec(2000),
        m_pointCons(3000),
        m_pointMaxVel(1000),
        m_pointTimes(2),
        m_reciAcc(2000),
        m_reciDec(2000),
        m_reciCons(3000),
        m_reciInterval(1000),
        m_reciMaxVel(1000),
        m_reciTimes(2)
    {}
    ~UiPTSLMotionData(){}
    bool m_isReci;
    double m_pointAcc;
    double m_pointDec;
    double m_pointCons;
    double m_pointMaxVel;
    int m_pointTimes;

    double m_reciAcc;
    double m_reciDec;
    double m_reciCons;
    double m_reciMaxVel;
    double m_reciInterval;
    int m_reciTimes;

    MotionRunSta m_motionSta;
};

class MOTIONPTSLSHARED_EXPORT UiMotionPTSL : public QWidget
{
    Q_OBJECT

public:
    explicit UiMotionPTSL(MotionPTSL *mp, QWidget *parent = 0);
    ~UiMotionPTSL();
    void updataUi(int axisInx);
    QList<UiPTSLMotionData*> uiDataList();
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void onCheckBoxReciClicked(bool checked);
    void onCssChanged(const QString &css);
    void onSpinBoxValueChanged(int value);
    void onDoubleSpinBoxValueChanged(double value);
    //void onTimerTimeOut();

private:
    void setupIcons(const QString &css);
private:
    Ui::UiMotionPTSL *ui;
    int m_axisCount;
    QList<UiPTSLMotionData*> m_uiDataList;
    MotionPTSL *q_ptr;
    //QTimer* m_timer;
};

#endif // UIMOTIONPTSL_H
