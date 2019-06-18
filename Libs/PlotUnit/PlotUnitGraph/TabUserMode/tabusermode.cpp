#include "tabusermode.h"
#include "ui_tabusermode.h"
#include "sevdevice.h"
#include "Option"
#include "gtutils.h"
#include "sdtglobaldef.h"
#include "usermodepanel.h"

#include <QDebug>
#include <QKeyEvent>
#include <QStyledItemDelegate>


#define ICON_NAME_SERVO_ON      "plot_son.png"
#define ICON_NAME_SERVO_OFF     "plot_soff.png"

#define GEN_CMD_PSRC_SEL        "gSevDrv.sev_obj.pos.seq.prm.psrc_sel"
#define GEN_CMD_VSRC_SEL        "gSevDrv.sev_obj.pos.seq.prm.vsrc_sel"
#define GEN_CMD_CSRC_SEL        "gSevDrv.sev_obj.pos.seq.prm.csrc_sel"

#define USER_MDOE_ADDR          28702
#define USER_PSRC_ADDR          13592
#define USER_VSRC_ADDR          13601
#define USER_CSRC_ADDR          13629

class UserModeCtlPrms
{
public:
  UserModeCtlPrms():m_curMode(GT::USER_MODE_IDLE)
  {

  }

  GT::UserModeType m_curMode;

};


TabUserMode::TabUserMode(const QString &name, SevDevice *sev, QWidget *parent) :
  ITabWidget(name,sev,parent),
  ui(new Ui::TabUserMode),
  m_currenAxis(0)
{
  ui->setupUi(this);

  ui->usrCtlPanel->setAxisCount(sev->axisNum());
  for(int i=0;i<sev->axisNum();i++)
  {
    UserModeCtlPrms *mode=new UserModeCtlPrms;
    m_dataList.append(mode);
  }

  OptFace *face=dynamic_cast<OptFace *>(OptContainer::instance()->optItem("optface"));
  connect(face,SIGNAL(faceCssChanged(QString)),this,SLOT(onCssChanged(QString)));
  setupIcons(face->css());

  connect(ui->usrCtlPanel,SIGNAL(checkChanged(quint16,int)),this,SLOT(onUsrCtlPanelCheckChanged(quint16,int)));
  connect(ui->usrCtlPanel,SIGNAL(modeChanged(quint16,int)),this,SLOT(onUsrCtlPanelModeChanged(quint16,int)));
  connect(ui->btn_saveMode, SIGNAL(clicked()), this, SLOT(onSaveBtnClicked()));
  //connect(ui->tbtn_plot_servoOnMode,SIGNAL(clicked(bool)),this,SLOT(onBtnServoOnClicked(bool)));
}

TabUserMode::~TabUserMode()
{
  qDebug()<<"TabModeCtl TabModeCtl TabModeCtl destruct ----->";
  GT::deepClearList(m_dataList);
  delete ui;

}

void TabUserMode::uiUpdate()
{
    quint16 curModeAxis = m_currenAxis;
  ui->stackedWidget_plot_mode->setCurrentIndex(ui->usrCtlPanel->mode(curModeAxis));
  if (m_sev->axisServoIsOn(curModeAxis)) {
      ui->usrCtlPanel->setEnabled(false);
  } else {
      ui->usrCtlPanel->setEnabled(true);
  }
  //qDebug()<<"update servo status"<<m_sev->aliasName()<<" cur mode axis = "<<curModeAxis<<" servo on = "<<m_sev->axisServoIsOn(curModeAxis);

}

void TabUserMode::resetUi()
{
    quint16 curModeAxis = m_currenAxis;
    for(int i=0;i<m_sev->axisNum();i++)
    {
        quint16 value = 0;
        m_sev->readFLASH16ByAddr(curModeAxis, USER_MDOE_ADDR, value);
        ui->usrCtlPanel->setMode(curModeAxis, value);
    }
}

void TabUserMode::setupIcons(const QString &css)
{

}

bool TabUserMode::eventFilter(QObject *obj, QEvent *event)
{
  if(event->type()==QEvent::KeyPress)
  {
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
    {
      return true;
    }
  }
  return QWidget::eventFilter(obj,event);
}

void TabUserMode::onCssChanged(const QString &css)
{
  setupIcons(css);
}
void TabUserMode::onModeSpinBoxValueChanged(int value)
{
  Q_UNUSED(value);
}

void TabUserMode::onModeDoubleSpinBoxValueChanged(double value)
{
    Q_UNUSED(value);
}
void TabUserMode::onUsrCtlPanelModeChanged(quint16 axis, int mode)
{
  if(mode == -1)
    return ;
  qDebug()<<axis<<mode;
  if(mode<ui->stackedWidget_plot_mode->count())
  {
    m_currenAxis=axis;
    UserModeCtlPrms* modePrms=m_dataList.at(axis);
    modePrms->m_curMode=(GT::UserModeType)mode;
    ui->stackedWidget_plot_mode->setCurrentIndex(mode);
    switch(mode)
    {
    case GT::USER_MODE_IDLE:break;
    case GT::USER_MODE_PT: {
        m_sev->genCmdWrite(GEN_CMD_PSRC_SEL, 2, axis);
        m_sev->setControlSrc(axis, GT::SEV_CTL_SRC_GLINK2);
    }
    break;
    case GT::USER_MODE_VSL: {
        m_sev->genCmdWrite(GEN_CMD_VSRC_SEL, 2, axis);
        m_sev->setControlSrc(axis, GT::SEV_CTL_SRC_GLINK2);
    }
    break;  
    case GT::USER_MODE_ANAVEL: {
        m_sev->genCmdWrite(GEN_CMD_VSRC_SEL, 3, axis);
        m_sev->setControlSrc(axis, GT::SEV_CTL_SRC_IO);
    }
    break;
    case GT::USER_MODE_DIRPULSE: {
        m_sev->genCmdWrite(GEN_CMD_PSRC_SEL, 3, axis);
        m_sev->setControlSrc(axis, GT::SEV_CTL_SRC_IO);
    }
    break;
    default: break;      
    }   
  }
}

void TabUserMode::onUsrCtlPanelCheckChanged(quint16 axis, int mode)
{

}

void TabUserMode::onSaveBtnClicked()
{
    for (int i = 0; i < m_sev->axisNum(); i++) {
        int axisMode = ui->usrCtlPanel->mode(i);
        m_sev->writeFLASH16ByAddr(i, USER_MDOE_ADDR, axisMode);
        switch(axisMode)
        {
        case GT::USER_MODE_IDLE:break;
        case GT::USER_MODE_PT: {
            m_sev->writeFLASH16ByAddr(i, USER_PSRC_ADDR, 2);
        }
        break;
        case GT::USER_MODE_VSL: {
            m_sev->writeFLASH16ByAddr(i, USER_VSRC_ADDR, 2);
        }
        break;
        case GT::USER_MODE_ANAVEL: {
            m_sev->writeFLASH16ByAddr(i, USER_VSRC_ADDR, 3);
        }
        break;
        case GT::USER_MODE_DIRPULSE: {
            m_sev->writeFLASH16ByAddr(i, USER_PSRC_ADDR, 3);
        }
        break;
        default: break;
        }
    }
}
  

//void TabUserMode::onBtnServoOnClicked(bool checked)
//{
//  if(!m_sev->isConnecting())
//  {
//    ui->tbtn_plot_servoOnMode->setChecked(false);
//    return ;
//  }
//  for(int i = 0;i<ui->usrCtlPanel->axisCount();i++)
//  {
//    if(ui->usrCtlPanel->isChecked(i))
//    {
//      m_sev->setAxisServoOn(i,checked);
//      ui->label_plot_servo_onoff->setText(tr("SEV ON"));
//      qDebug()<<"setServoOn "<<i<<checked;
//      qDebug()<<"----------sev axis is on"<<m_sev->axisServoIsOn(i)<<"-----------";
//    }
//  }
//  if (checked && ui->stackedWidget_plot_mode->currentIndex() == 13) {
//      for(int i = 0;i<ui->usrCtlPanel->axisCount();i++)
//      {
//        if(ui->usrCtlPanel->isChecked(i))
//        {
//            m_finishList.append(0);
//        }
//      }
//      connect(&m_timer, SIGNAL(timeout()), this, SLOT(onCheckingAutoTurning()));
//      m_timer.start(1000);
//  }
//}

//void TabUserMode::onCheckingAutoTurning()
//{
//    for (int i = 0; i < ui->usrCtlPanel->axisCount(); i++) {
//        int count = 0;
//        if (ui->usrCtlPanel->isChecked(i)) {
//            bool isOk;
//            quint64 finish = m_sev->genCmdRead(GEN_CMD_AUT_FINISH, i, isOk);
//            m_finishList.replace(count, finish);
//            if (m_finishList.at(count) == 1) {
//                UserModeCtlPrms* modePrms = m_dataList.at(i);
//                modePrms->m_autJm = m_sev->genCmdRead(GEN_CMD_MOTOR_JM, i, isOk);
//                ui->label_mode_aut_Jm->setText(QString::number(modePrms->m_autJm));
//            }
//            count++;
//        }
//    }
//    for (int i = 0; i < m_finishList.count(); i++) {
//        if (m_finishList.at(i) == 0) {
//            break;
//        }
//        m_timer.stop();
//    }
//}
