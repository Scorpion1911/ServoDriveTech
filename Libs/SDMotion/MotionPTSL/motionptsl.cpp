#include "motionptsl.h"
#include "uimotionptsl.h"
#include "imotion_p.h"
#include "sevdevice.h"
#include "gtutils.h"
#include "ptplanmotion.h"

#include <QListWidget>
#include <QDebug>

#define CMD_PT_CMDSRC              "gSevDrv.sev_obj.pos.seq.prm.psrc_sel"

class MotionPTSLPrivate:public IMotionPrivate
{
    Q_DECLARE_PUBLIC(MotionPTSL)
public:
    MotionPTSLPrivate(){}
    ~MotionPTSLPrivate(){}
protected:
    qint16 m_cmdSrc;
    QList<PTPlanMotion *> m_PTPlanList;
};

MotionPTSL::MotionPTSL(QListWidget *axisListWidget, SevDevice *sev, const QString &name, QObject *parent):
    IMotion(*(new MotionPTSLPrivate),parent)
{
    Q_D(MotionPTSL);
    setSevDevice(sev);
    setMotionType(MOTION_TYPE_PTSL);
    setName(name);
    d->m_axisListWidget = axisListWidget;
    UiMotionPTSL *uiPTSL = new UiMotionPTSL(this, 0);
    d->m_ui = uiPTSL;
    d->m_motionUnFinishVector.clear();
    for (int i = 0; i < sev->axisNum(); i++) {
        PTPlanMotion *pt = new PTPlanMotion(i, sev, uiPTSL->uiDataList().at(i));
        d->m_PTPlanList.append(pt);
        connect(pt, SIGNAL(motionFinish(quint16)), this, SLOT(onMotionFinish(quint16)));
        connect(pt, SIGNAL(progressValueChanged(quint16,int)), this, SIGNAL(progressValueChanged(quint16,int)));
    }
//    connect(this, SIGNAL(motionFinish(quint16)), this, SLOT(onMotionFinish(quint16)));
//    m_count = 0;
//    m_timer.setTimerType(Qt::PreciseTimer);
//    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
}

MotionPTSL::~MotionPTSL()
{
    Q_D(MotionPTSL);
    delete d->m_ui;
}

void MotionPTSL::movePrepare(quint16 axisInx)
{
    Q_D(MotionPTSL);
    d->m_PTPlanList.at(axisInx)->movePrepare();
    d->m_motionUnFinishVector.clear();
}

bool MotionPTSL::move(quint16 axisInx)
{
    Q_D(MotionPTSL);
    if (!d->m_sev->axisServoIsOn(axisInx)) {
        return false ;
    }

    d->m_motionUnFinishVector.append(axisInx);
    d->m_PTPlanList.at(axisInx)->move();
    d->m_ui->setEnabled(false);
    return true;
}

bool MotionPTSL::stop(quint16 axisInx)
{
    Q_D(MotionPTSL);
    d->m_PTPlanList.at(axisInx)->stop();
    d->m_ui->setEnabled(true);
    return true;
}

void MotionPTSL::updateAxisUi(quint16 axisInx)
{
    UiMotionPTSL *ui = UiMotion();
    ui->updataUi(axisInx);
}

void MotionPTSL::setMode()
{
    Q_D(MotionPTSL);
    for (int i = 0; i < d->m_axisListWidget->count(); i++) {
        if (d->m_axisListWidget->item(i)->isSelected()) {
            d->m_sev->setCurrentTaskServoMode(i, GT::MODE_PT);
        }
    }
}

void MotionPTSL::setCmdSrc(quint16 axisInx)
{
    Q_D(MotionPTSL);
    bool isOk;
    d->m_cmdSrc = d->m_sev->genCmdRead(CMD_PT_CMDSRC, axisInx, isOk);
    d->m_sev->genCmdWrite(CMD_PT_CMDSRC, 4, axisInx);
}

void MotionPTSL::resetCmdSrc(quint16 axisInx)
{
    Q_D(MotionPTSL);
    d->m_sev->genCmdWrite(CMD_PT_CMDSRC, d->m_cmdSrc, axisInx);
}

UiMotionPTSL *MotionPTSL::UiMotion()
{
    Q_D(MotionPTSL);
    UiMotionPTSL *ui = qobject_cast<UiMotionPTSL *>(d->m_ui);
    return ui;
}

void MotionPTSL::onMotionFinish(quint16 axisInx)
{
    Q_UNUSED(axisInx);
}

void MotionPTSL::onTimerOut()
{

}
