#ifndef MOTIONPTSL_H
#define MOTIONPTSL_H

#include "motionptsl_global.h"
#include "imotion.h"

#include <QTimer>

class MotionPTSLPrivate;
class UiMotionPTSL;

class MOTIONPTSLSHARED_EXPORT MotionPTSL : public IMotion
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MotionPTSL)
public:
    MotionPTSL(QListWidget *axisListWidget, SevDevice *sev, const QString &name = "ptsl", QObject *parent = 0);
    ~MotionPTSL();

    void movePrepare(quint16 axisInx) Q_DECL_OVERRIDE;
    bool move(quint16 axisInx) Q_DECL_OVERRIDE;
    bool stop(quint16 axisInx) Q_DECL_OVERRIDE;
    void updateAxisUi(quint16 axisInx) Q_DECL_OVERRIDE;
    void setMode() Q_DECL_OVERRIDE;
    void setCmdSrc(quint16 axisInx) Q_DECL_OVERRIDE;
    void resetCmdSrc(quint16 axisInx) Q_DECL_OVERRIDE;
    UiMotionPTSL * UiMotion();

private slots:
    void onMotionFinish(quint16 axisInx);
    void onTimerOut();

private:
    QTimer m_timer;
    int m_count;
};

#endif // MOTIONPTSL_H
