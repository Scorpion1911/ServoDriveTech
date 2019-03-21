#include "graphstatus136.h"
#include "ui_graphstatus136.h"
#include "igraphstatus_p.h"
#include "ledalarm.h"
#include "gtutils.h"
#include "sdtglobaldef.h"
#include "sevdevice.h"
#include "iuiwidget.h"
#include "alarmhistorydia136.h"

#include <QGridLayout>
#include <QTreeWidget>
#include <QDebug>

#define CURR_STATE_INX    0
#define ADC_VDC_INX       1
#define ALM_FLAG_INX      2
#define ALM_CODE_ALL_INX  3
#define CMD_ID_INX        4
#define CURR_ID_INX       5
#define USR_MODE_INX      6



class GraphStatus136Private:public IGraphStatusPrivate
{
  Q_DECLARE_PUBLIC(GraphStatus136)
public:
  GraphStatus136Private(){}
  ~GraphStatus136Private(){}
};

GraphStatus136::GraphStatus136(QWidget *parent) :
  IGraphStatus(*(new GraphStatus136Private),parent),
  ui(new Ui::GraphStatus136)
{
  ui->setupUi(this);
  ui->btn_statusClearAlarm->setText(tr("ClearAlarm"));
  connect(ui->btn_statusClearAlarm,SIGNAL(clicked(bool)),this,SLOT(onBtnClearAlarmClicked()));
  connect(ui->btn_status_history, SIGNAL(clicked()), this, SLOT(onBtnAlarmHistoryClicked()));
}

GraphStatus136::~GraphStatus136()
{
  delete ui;
}
void GraphStatus136::setUiVersionName()
{
  Q_D(GraphStatus136);
  d->m_versionName="V136";
}

void GraphStatus136::setupDataMappings()
{

}

void GraphStatus136::setDeviceStatusIconByCss(const QString &css)
{
  QString iconpath=GTUtils::customPath()+"/option/style/"+css+"/icon/status_device.png";
  ui->label_statusIcon->setPixmap(QPixmap(iconpath));
}

QWidget *GraphStatus136::alarmBackgroundWidget()
{
  return ui->widget_statusAlarm;
}

void GraphStatus136::addLedErrorToUi()
{
  Q_D(GraphStatus136);
  ui->verticalLayout_statusInfo->insertWidget(0,d->m_ledFlag);
  ui->verticalLayout_statusInfo->setContentsMargins(20,30,20,20);
  ui->verticalLayout_statusInfo->setStretch(0,0);
  ui->verticalLayout_statusInfo->setStretch(1,1);
  ui->verticalLayout_statusInfo->setStretch(2,0);
  ui->verticalLayout_statusInfo->setStretch(3,1);
}

quint32 GraphStatus136::alarmCode()
{
  Q_D(GraphStatus136);
  quint32 code=0;
  double value=0;
  value=d->m_treeWidget->topLevelItem(ALM_CODE_ALL_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
  code=value;
  return code;
}

bool GraphStatus136::hasError()
{
  Q_D(GraphStatus136);
  qint16 code=0;
  double value=0;
  value=d->m_treeWidget->topLevelItem(ALM_FLAG_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
  code=value;
  return (bool)code;
}

void GraphStatus136::updateUiLabelText()
{
  Q_D(GraphStatus136);
  //从1开始的
  qint16 currState=d->m_treeWidget->topLevelItem(CURR_STATE_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble()-1;
  if(currState<d->m_sevStatusText.count())
    ui->label_statusValue_sevStatus->setText(d->m_sevStatusText.at(currState));

  double vdc;
  double vdcGain=1;
  vdcGain=d->m_treeWidget->topLevelItem(ADC_VDC_INX)->text(GT::COL_PAGE_TREE_SCALE).toDouble();
  vdc=d->m_treeWidget->topLevelItem(ADC_VDC_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble()/vdcGain;
  ui->label_statusValue_busVol->setText(QString::number(vdc,'f',3));

  qint16 cmdMode=0;
  cmdMode=d->m_treeWidget->topLevelItem(CMD_ID_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
  if(cmdMode<d->m_sevModeText.count())
    ui->label_statusValue_cmdMode->setText(d->m_sevModeText.at(cmdMode));

  qint16 currMode=0;
  currMode=d->m_treeWidget->topLevelItem(CURR_ID_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
  if(currMode<d->m_sevModeText.count())
    ui->label_statusValue_curMode->setText(d->m_sevModeText.at(currMode));

  qint16 usrMode=0;
  usrMode=d->m_treeWidget->topLevelItem(USR_MODE_INX)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
  if(usrMode<d->m_sevUsrModeText.count())
    ui->label_statusValue_usrMode->setText(d->m_sevUsrModeText.at(usrMode));

}

void GraphStatus136::onBtnClearAlarmClicked()
{
  Q_D(GraphStatus136);
  d->m_dev->clearAlarm(d->m_uiWidget->uiIndexs().axisInx);
  qDebug()<<"clear alarm";
}

void GraphStatus136::onBtnAlarmHistoryClicked()
{
    Q_D(GraphStatus136);
    QTreeWidgetItem *item = d->m_treeWidget->topLevelItem(ALM_CODE_ALL_INX);
    AlarmHistoryDia136 historyDia = AlarmHistoryDia136(d->m_uiWidget->uiIndexs().axisInx, d->m_dev, item, 0);
    historyDia.exec();
}

