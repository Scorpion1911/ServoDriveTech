#include "graphvelocity133.h"
#include "igraphvelocity_p.h"
#include "boxitemmapping.h"
#include "widgetitem.h"
#include "velpid133.h"
#include "sevdevice.h"
#include "deviceconfig.h"
#include "iuiwidget.h"
#include "anchoritemhelper.h"
#include "SDTGraphicsItems"
#include "Option"
#include "VelAutoTurning/velautoturning133.h"

#include <QRadioButton>
#include <QDebug>
#include <QGraphicsSimpleTextItem>

#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QGroupBox>

#define MOT_NOS_KEY_NAME "gSevDrv.sev_obj.cur.mot.Nos_1"
#define POW2_24 16777216

#define PID_POS_X -100
#define PID_POS_Y -240

#define FN_FST_5_INX      0
#define TI_FST_5_INX      1
#define ABS_RAT_5_INX     2
#define POS_RAT_5_INX     3
#define NEG_RAT_5_INX     4
#define FN_SEC_5_INX      5
#define TI_SEC_5_INX      6
#define KVI_SW_EN_INX     7
#define KVI_SW_TIM_INX    8
#define KVI_SW_SPDU_INX   9
#define KVI_SW_SPDL_INX   10
#define VEL_DIR_INDEX     11
#define VEL_AMP_MAX       12
#define VEL_CYCLE_TIME    13
#define VEL_AUTOTUN_MODE  14

class GraphVelocity133Private:public IGraphVelocityPrivate
{
  Q_DECLARE_PUBLIC(GraphVelocity133)
public:
  GraphVelocity133Private(){}
  ~GraphVelocity133Private(){}

protected:
  VelPid133 *m_pid133;
  VelAutoTurning133 *m_velAuto;

};

GraphVelocity133::GraphVelocity133(QWidget *parent) :
  IGraphVelocity(*(new GraphVelocity133Private),parent)
{
}

GraphVelocity133::~GraphVelocity133()
{
//  delete ui;
  qDebug()<<"GraphVelocity133 destruct-->";
  Q_D(GraphVelocity133);
  delete d->m_velAuto;
}
void GraphVelocity133::setCustomVisitActive(IUiWidget *uiWidget)
{
    Q_UNUSED(uiWidget);
    Q_D(GraphVelocity133);
    d->m_velAuto = new VelAutoTurning133(d->m_dev, uiWidget->uiIndexs().axisInx);
    connect(d->m_velAuto, SIGNAL(progressChanged(int)), this, SLOT(onAutoTurningProgressValueChanged(int)));
    connect(d->m_velAuto, SIGNAL(autoTurnigFinish(bool)), this, SLOT(onAutoTurningFinish(bool)));
}
void GraphVelocity133::setUiVersionName()
{
  Q_D(GraphVelocity133);
  d->m_versionName="V133";
  setObjectName("GraphVelocity133");
}

void GraphVelocity133::setupDataMappings()
{
  Q_D(GraphVelocity133);

  d->m_mapping->insertBox2Item(d->m_pEdit,d->m_treeWidget->topLevelItem(FN_FST_5_INX));
  d->m_mapping->insertBox2Item(d->m_iEdit,d->m_treeWidget->topLevelItem(TI_FST_5_INX));

  d->m_mapping->insertBox2Item(d->m_maxTqEdit,d->m_treeWidget->topLevelItem(ABS_RAT_5_INX));
  d->m_mapping->insertBox2Item(d->m_maxTqEdit_P,d->m_treeWidget->topLevelItem(POS_RAT_5_INX));
  d->m_mapping->insertBox2Item(d->m_maxTqEdit_N,d->m_treeWidget->topLevelItem(NEG_RAT_5_INX));
  d->m_mapping->insertBox2Item(d->m_spdBox, d->m_treeWidget->topLevelItem(VEL_AMP_MAX));
  d->m_mapping->insertBox2Item(d->m_cycleTimeBox, d->m_treeWidget->topLevelItem(VEL_CYCLE_TIME));


  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(FN_FST_5_INX),d->m_pEdit);
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(TI_FST_5_INX),d->m_iEdit);

  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(ABS_RAT_5_INX),d->m_maxTqEdit);
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(POS_RAT_5_INX),d->m_maxTqEdit_P);
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(NEG_RAT_5_INX),d->m_maxTqEdit_N);
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(VEL_AMP_MAX), d->m_spdBox);
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(VEL_CYCLE_TIME), d->m_cycleTimeBox);

  //sec pid
  d->m_mapping->insertBox2Item(d->m_pid133->boxP2(),d->m_treeWidget->topLevelItem(FN_SEC_5_INX));
  d->m_mapping->insertBox2Item(d->m_pid133->boxI2(),d->m_treeWidget->topLevelItem(TI_SEC_5_INX));
  d->m_mapping->insertBox2Item(d->m_pid133->boxTime(),d->m_treeWidget->topLevelItem(KVI_SW_TIM_INX));
  d->m_mapping->insertBox2Item(d->m_pid133->boxSpdu(),d->m_treeWidget->topLevelItem(KVI_SW_SPDU_INX));
  d->m_mapping->insertBox2Item(d->m_pid133->boxSpdl(),d->m_treeWidget->topLevelItem(KVI_SW_SPDL_INX));

  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(FN_SEC_5_INX),d->m_pid133->boxP2());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(TI_SEC_5_INX),d->m_pid133->boxI2());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(KVI_SW_TIM_INX),d->m_pid133->boxTime());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(KVI_SW_SPDU_INX),d->m_pid133->boxSpdu());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(KVI_SW_SPDL_INX),d->m_pid133->boxSpdl());

}

void GraphVelocity133::createPidItem()
{
  Q_D(GraphVelocity133);

  QWidget *wout = new QWidget;
  QVBoxLayout *voutLayout = new QVBoxLayout(wout);
  QSpacerItem *verticalSpacer1 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
  QSpacerItem *verticalSpacer2 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

  QWidget *w = new QWidget;
  w->setObjectName("widget_velPid");
  QVBoxLayout *vlayout = new QVBoxLayout(w);

  d->m_pid133 = new VelPid133(w);
  connect(d->m_pid133,SIGNAL(comboBoxIndexChanged(int)),this,SLOT(onPidComboBoxSWIndexChanged(int)));
  vlayout->addWidget(d->m_pid133);
  w->setLayout(vlayout);

  voutLayout->addSpacerItem(verticalSpacer1);
  voutLayout->addWidget(w);
  voutLayout->addSpacerItem(verticalSpacer2);
  voutLayout->setContentsMargins(0,0,0,0);
  wout->setLayout(voutLayout);

  d->m_pEdit=d->m_pid133->boxP1();//p1
  d->m_iEdit=d->m_pid133->boxI1();//i1


  d->m_UPID=new WidgetItem;
  d->m_UPID->setWidget(wout,true);
  d->m_scene->addItem(d->m_UPID);//take ownership

  d->m_widgetItemList.append(d->m_UPID);
}

void GraphVelocity133::createAutoTuningWidget()
{
    Q_D(GraphVelocity133);
    d->m_autoTnItem = new WidgetItem;
    QWidget *w = new QWidget;
    w->setObjectName("widget_spdCtl_autoTurning");
    QVBoxLayout *vlayout = new QVBoxLayout(w);
    QHBoxLayout *hlayout = new QHBoxLayout();
    QLabel *label = new QLabel(tr("AutoTurning"),w);
    label->setAlignment(Qt::AlignCenter);
    label->setObjectName("label_spdCtl_autoTurning");
    d->m_spdBox = new QDoubleSpinBox(w);
    d->m_spdBox->setMinimum(0);
    d->m_spdBox->setMaximum(32767);
    d->m_spdBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    d->m_bar = new QProgressBar(w);
    d->m_bar->setValue(0);
    d->m_bar->setMaximum(100);
    d->m_bar->setVisible(false);
    d->m_btnAutoStart = new QPushButton(w);
    connect(d->m_btnAutoStart,SIGNAL(clicked(bool)),this,SLOT(onBtnAutoStartClicked(bool)));
    d->m_btnAutoStart->setCheckable(true);
    d->m_btnAutoStart->setText(tr("auto turning start"));
    d->m_btnAutoStart->setObjectName("btn_spdCtl_autoStart");
    QLabel *label5 = new QLabel(tr("Speed Amp(rpm)"));

    QHBoxLayout *hLayout2 = new QHBoxLayout();
    QGroupBox *box = new QGroupBox(tr("Jm Indentification"));
    d->m_jmYesBtn = new QRadioButton;
    d->m_jmYesBtn->setText(tr("Yes"));
    d->m_jmYesBtn->setChecked(true);
    d->m_jmNoBtn = new QRadioButton;
    d->m_jmNoBtn->setText(tr("No"));
    d->m_jmNoBtn->setChecked(false);
    connect(d->m_jmYesBtn, SIGNAL(clicked(bool)), this, SLOT(onJmRadioBtnClicked(bool)));
    connect(d->m_jmNoBtn, SIGNAL(clicked(bool)), this, SLOT(onJmRadioBtnClicked(bool)));
    hLayout2->addWidget(d->m_jmYesBtn);
    hLayout2->addWidget(d->m_jmNoBtn);
    box->setLayout(hLayout2);

    QHBoxLayout *hLayout4 = new QHBoxLayout();
    d->m_cycleLable = new QLabel(tr("Cycle Time(ms):"));
    d->m_cycleTimeBox = new QDoubleSpinBox;
    d->m_cycleTimeBox->setMinimum(0);
    d->m_cycleTimeBox->setMaximum(32767);
    d->m_cycleTimeBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    hLayout4->addWidget(d->m_cycleLable);
    hLayout4->addWidget(d->m_cycleTimeBox);

    QHBoxLayout *hLayout3 = new QHBoxLayout();
    d->m_jmTextLabel = new QLabel(tr("Jm:"));
    d->m_jmLabel = new QLabel;
    hLayout3->addWidget(d->m_jmTextLabel);
    hLayout3->addWidget(d->m_jmLabel);

    hlayout->addWidget(label5);
    hlayout->addWidget(d->m_spdBox);
    vlayout->addWidget(label);
    vlayout->addWidget(box);
    vlayout->addLayout(hlayout);
    vlayout->addLayout(hLayout4);
    vlayout->addLayout(hLayout3);
    vlayout->addWidget(d->m_bar);
    vlayout->addWidget(d->m_btnAutoStart);
    w->setLayout(vlayout);
    d->m_autoTnItem->setWidget(w,true);
    d->m_scene->addItem(d->m_autoTnItem);
    d->m_widgetItemList.append(d->m_autoTnItem);
}

void GraphVelocity133::createArrowItems()
{
    Q_D(GraphVelocity133);
    d->m_A0=new ArrowItem(d->m_Tstart->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_dirItem->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A13 = new ArrowItem(d->m_dirItem->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UIF->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A1=new ArrowItem(d->m_UIF->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A2=new ArrowItem(d->m_USUM->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UPID->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A3=new ArrowItem(d->m_UPID->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USATN->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A4=new ArrowItem(d->m_USATN->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UOF->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A5=new ArrowItem(d->m_UOF->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_Tend->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A6=new ArrowItem(d->m_T0->pointF(WidgetItem::POINT_TYPE_LEFT),d->m_UVB->pointF(WidgetItem::POINT_TYPE_RIGHT));
    d->m_A7=new ArrowItem(d->m_UVB->pointF(WidgetItem::POINT_TYPE_LEFT),d->m_USUM->pointF(WidgetItem::POINT_TYPE_BOTTOM),ArrowItem::ARROW_TYPE_CORNER,"-");

    d->m_scene->addItem(d->m_A0);
    d->m_scene->addItem(d->m_A13);
    d->m_scene->addItem(d->m_A1);
    d->m_scene->addItem(d->m_A2);
    d->m_scene->addItem(d->m_A3);
    d->m_scene->addItem(d->m_A4);
    d->m_scene->addItem(d->m_A5);
    d->m_scene->addItem(d->m_A6);
    d->m_scene->addItem(d->m_A7);

    d->m_arrowList.append(d->m_A0);
    d->m_arrowList.append(d->m_A13);
    d->m_arrowList.append(d->m_A1);
    d->m_arrowList.append(d->m_A2);
    d->m_arrowList.append(d->m_A3);
    d->m_arrowList.append(d->m_A4);
    d->m_arrowList.append(d->m_A5);
    d->m_arrowList.append(d->m_A6);
    d->m_arrowList.append(d->m_A7);
}

void GraphVelocity133::onPidComboBoxSWIndexChanged(int index)
{
  Q_D(GraphVelocity133);

  d->m_treeWidget->topLevelItem(KVI_SW_EN_INX)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(index));
  qDebug()<<"velocity index = "<<index;
}

void GraphVelocity133::onBtnAutoStartClicked(bool checked)
{
    Q_D(GraphVelocity133);
    if(d->m_dev->isConnecting() == false)
      return ;

    if(checked)
    {
      bool ok = false;
      d->m_velAuto->setVelAmp(d->m_treeWidget->topLevelItem(VEL_AMP_MAX)->text(GT::COL_PAGE_TREE_VALUE).toULongLong());
      d->m_velAuto->setCycleTime(d->m_treeWidget->topLevelItem(VEL_CYCLE_TIME)->text(GT::COL_PAGE_TREE_VALUE).toULongLong());
      d->m_velAuto->setJmStatus(d->m_jmYesBtn->isChecked());
      ok = d->m_velAuto->start();

      if(!ok)
      {
        d->m_velAuto->stop();
        QMessageBox::warning(0,tr("Error"),tr("set servo on error !"));
      }
    }
    else
    {
      d->m_velAuto->stop();
    }

}

void GraphVelocity133::onAutoTurningProgressValueChanged(int value)
{
  setAutoTurningProgressBarValue(value);
}

void GraphVelocity133::onAutoTurningFinish(bool finish)
{
  Q_D(GraphVelocity133);
  if(finish)
  {
    setBtnAutoTurningUiOn(false);
    setProgressBarVisible(false);
    //更新值到kp kn ki kd
    qreal fn = d->m_velAuto->autoTnFn();


    d->m_pEdit->setValue(fn);
    d->m_mapping->syncBoxText2Item(d->m_pEdit);

    if (d->m_velAuto->isJm()) {
        d->m_jmLabel->setText(QString::number(d->m_velAuto->getJm()));
    }
    OptFace *face = dynamic_cast<OptFace*>(OptContainer::instance()->optItem("optface"));
    face->setEditTextStatus(d->m_spdBox,OptFace::EDIT_TEXT_STATUS_READY);

  }
  else
  {
    QMessageBox::warning(0,tr("Error"),tr("auto turning fail !\nyou need to stop autoturning and try again!"));
  }
}

void GraphVelocity133::onRadioBtnClicked(bool checked)
{
    Q_D(GraphVelocity133);
    Q_UNUSED(checked);
    int dir;
    if (d->m_cwRBtn->isChecked()) {
        dir = 0;
    } else {
        dir = 1;
    }
    d->m_treeWidget->topLevelItem(VEL_DIR_INDEX)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(dir));
}

void GraphVelocity133::onJmRadioBtnClicked(bool checked)
{
    Q_D(GraphVelocity133);
    Q_UNUSED(checked);
    qint16 value = d->m_treeWidget->topLevelItem(VEL_AUTOTUN_MODE)->text(GT::COL_PAGE_TREE_VALUE).toShort();
    int jDit;
    if (d->m_jmYesBtn->isChecked()) {
        jDit = value & 0xFFF7;
    } else {
        jDit = value | 0x0008;
    }
    d->m_jmLabel->setVisible(d->m_jmYesBtn->isChecked());
    d->m_jmTextLabel->setVisible(d->m_jmYesBtn->isChecked());
    d->m_cycleLable->setVisible(d->m_jmYesBtn->isChecked());
    d->m_cycleTimeBox->setVisible(d->m_jmYesBtn->isChecked());
    d->m_treeWidget->topLevelItem(VEL_DIR_INDEX)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(jDit));
}

QPointF GraphVelocity133::pidInitPos()
{
  return QPointF(PID_POS_X,PID_POS_Y);
}
quint16 GraphVelocity133::readNos(const QString &key)
{
  Q_D(GraphVelocity133);
  bool isOk=true;
  quint16 nos=0;
  nos = d->m_dev->genCmdReadNos(d->m_uiWidget->uiIndexs().axisInx,isOk);
  //nos=d->m_dev->genCmdRead(key,d->m_uiWidget->uiIndexs().axisInx,isOk);
  return nos;
}

void GraphVelocity133::syncTreeDataToUiFace()
{
  Q_D(GraphVelocity133);

  //读变量，修改动态增益
  quint16 nos = readNos(MOT_NOS_KEY_NAME);
  if(nos == 0)
    nos = 1;
  double n = nos;
  double k = POW2_24/n;
  d->m_treeWidget->topLevelItem(KVI_SW_SPDU_INX)->setText(GT::COL_PAGE_TREE_SCALE,QString::number(k,'f',3));
  d->m_treeWidget->topLevelItem(KVI_SW_SPDL_INX)->setText(GT::COL_PAGE_TREE_SCALE,QString::number(k,'f',3));

  IGraphVelocity::syncTreeDataToUiFace();

  quint16 sw_en = d->m_treeWidget->topLevelItem(KVI_SW_EN_INX)->text(GT::COL_PAGE_TREE_VALUE).toUShort();
  d->m_pid133->setCurrentPidSegment(sw_en);

  qDebug()<<"sw_en = "<<sw_en;

  qint16 dir = 0;
  dir = d->m_treeWidget->topLevelItem(VEL_DIR_INDEX)->text(GT::COL_PAGE_TREE_VALUE).toShort();
  if (dir == 0) {
    d->m_cwRBtn->setChecked(true);
  } else {
    d->m_ccwRBtn->setChecked(true);
  }

  qint16 jDit = 0;
  jDit = (d->m_treeWidget->topLevelItem(VEL_AUTOTUN_MODE)->text(GT::COL_PAGE_TREE_VALUE).toShort()) & 0x0008;
  if (dir == 1) {
    d->m_jmNoBtn->setChecked(true);
    onJmRadioBtnClicked(true);
  } else {
    d->m_jmYesBtn->setChecked(true);
    onJmRadioBtnClicked(false);
  }
}

void GraphVelocity133::installDoubleSpinBoxEventFilter()
{
  Q_D(GraphVelocity133);
  IGraphVelocity::installDoubleSpinBoxEventFilter();

  d->m_pid133->boxI2()->installEventFilter(this);
  d->m_pid133->boxP2()->installEventFilter(this);
  d->m_pid133->boxTime()->installEventFilter(this);
  d->m_pid133->boxSpdl()->installEventFilter(this);
  d->m_pid133->boxSpdu()->installEventFilter(this);
}
void GraphVelocity133::setDoubleSpinBoxConnections()
{
  Q_D(GraphVelocity133);
  IGraphVelocity::setDoubleSpinBoxConnections();
//  connect(d->m_pEdit,SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_pid133->boxI2(),SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_pid133->boxP2(),SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_pid133->boxTime(),SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_pid133->boxSpdl(),SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
  connect(d->m_pid133->boxSpdu(),SIGNAL(editingFinished()),this,SLOT(onDoubleSpinBoxFocusOut()));
}

void GraphVelocity133::setUpItemPosAnchors()
{
    Q_D(GraphVelocity133);
    d->m_UPID->setPos(pidInitPos());
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USATN,AnchorItemHelper::AnchorRight,1.5*d->m_USATN->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USATN,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_USATN,d->m_UOF,AnchorItemHelper::AnchorRight,1.5*d->m_UOF->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_USATN,d->m_UOF,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UOF,d->m_Tend,AnchorItemHelper::AnchorRight,140);
    d->m_anchorHelper->addAnchor(d->m_UOF,d->m_Tend,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USUM,AnchorItemHelper::AnchorLeft,-2*d->m_USUM->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USUM,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_USUM,d->m_UIF,AnchorItemHelper::AnchorLeft,-1*d->m_UIF->boundingRect().width()-d->m_USUM->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_USUM,d->m_UIF,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UIF,d->m_dirItem,AnchorItemHelper::AnchorLeft,-0.8*d->m_dirItem->boundingRect().width()-d->m_UIF->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UIF,d->m_dirItem,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_dirItem,d->m_Tstart,AnchorItemHelper::AnchorLeft,-100);
    d->m_anchorHelper->addAnchor(d->m_dirItem,d->m_Tstart,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UVB,AnchorItemHelper::AnchorHorizontalCenter,50);
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UVB,AnchorItemHelper::AnchorBottom,2*d->m_UVB->boundingRect().height());

    d->m_anchorHelper->addAnchor(d->m_UVB,d->m_T0,AnchorItemHelper::AnchorRight,50);
    d->m_anchorHelper->addAnchor(d->m_UVB,d->m_T0,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_Tstart,d->m_TextStart,AnchorItemHelper::AnchorLeft,1.2*d->m_Tstart->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_Tstart,d->m_TextStart,AnchorItemHelper::AnchorBottom,-15);

    d->m_anchorHelper->addAnchor(d->m_Tend,d->m_TextEnd,AnchorItemHelper::AnchorRight,-1.2*d->m_Tend->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_Tend,d->m_TextEnd,AnchorItemHelper::AnchorBottom,-15);


    //saturation config widget
    double wp=dynamic_cast<FrameItemWidget*>(d->m_UFRAME->widget())->getPosPercent();
    double hp=dynamic_cast<FrameItemWidget*>(d->m_UFRAME->widget())->getHPercent();
    double oft=-1*(wp-0.5)*d->m_UFRAME->boundingRect().width();
    d->m_anchorHelper->addAnchor(d->m_USATN,d->m_UFRAME,AnchorItemHelper::AnchorHorizontalCenter,oft);
    d->m_anchorHelper->addAnchor(d->m_USATN,d->m_UFRAME,AnchorItemHelper::AnchorBottom,-1*d->m_USATN->boundingRect().height()-10);

    oft=-1*((1-hp)/2-hp)*d->m_UFRAME->boundingRect().height();
    d->m_anchorHelper->addAnchor(d->m_UFRAME,d->m_UMAXTQ,AnchorItemHelper::AnchorLeft,20);
    d->m_anchorHelper->addAnchor(d->m_UFRAME,d->m_UMAXTQ,AnchorItemHelper::AnchorVerticalCenter,oft);

    d->m_anchorHelper->addAnchor(d->m_UMAXTQ,d->m_T1,AnchorItemHelper::AnchorRight,30);
    d->m_anchorHelper->addAnchor(d->m_UMAXTQ,d->m_T1,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_T1,d->m_UMAXTQ_P,AnchorItemHelper::AnchorRight,d->m_UMAXTQ_P->boundingRect().width()+30);
    d->m_anchorHelper->addAnchor(d->m_T1,d->m_UMAXTQ_P,AnchorItemHelper::AnchorVerticalCenter,-1*d->m_UMAXTQ->boundingRect().height()/2-10);

    d->m_anchorHelper->addAnchor(d->m_T1,d->m_UMAXTQ_N,AnchorItemHelper::AnchorRight,d->m_UMAXTQ_N->boundingRect().width()+30);
    d->m_anchorHelper->addAnchor(d->m_T1,d->m_UMAXTQ_N,AnchorItemHelper::AnchorVerticalCenter,1*d->m_UMAXTQ->boundingRect().height()/2+10);

    d->m_anchorHelper->addAnchor(d->m_UMAXTQ_P,d->m_T2,AnchorItemHelper::AnchorRight,30);
    d->m_anchorHelper->addAnchor(d->m_UMAXTQ_P,d->m_T2,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UMAXTQ_N,d->m_T3,AnchorItemHelper::AnchorRight,30);
    d->m_anchorHelper->addAnchor(d->m_UMAXTQ_N,d->m_T3,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_T2,d->m_TextMaxTqPositive,AnchorItemHelper::AnchorRight,d->m_TextMaxTqPositive->boundingRect().width()+5);
    d->m_anchorHelper->addAnchor(d->m_T2,d->m_TextMaxTqPositive,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_T3,d->m_TextMaxTqNegative,AnchorItemHelper::AnchorRight,d->m_TextMaxTqNegative->boundingRect().width()+5);
    d->m_anchorHelper->addAnchor(d->m_T3,d->m_TextMaxTqNegative,AnchorItemHelper::AnchorVerticalCenter);
    d->m_anchorHelper->addAnchor(d->m_Tstart, d->m_autoTnItem, AnchorItemHelper::AnchorLeft);
    d->m_anchorHelper->addAnchor(d->m_Tstart, d->m_autoTnItem, AnchorItemHelper::AnchorBottom, 300);
}

void GraphVelocity133::createVelDirItem()
{
    Q_D(GraphVelocity133);
    d->m_dirItem = new WidgetItem;
    QWidget *dirWidget = new QWidget;
    dirWidget->setObjectName("widget_velDir");
    QVBoxLayout *vlayout = new QVBoxLayout(dirWidget);
    QLabel *label = new QLabel(tr("DirCtl"),dirWidget);
    label->setAlignment(Qt::AlignCenter);
    label->setObjectName("label_velDirControl");
    QHBoxLayout *hlayout = new QHBoxLayout;
    d->m_cwRBtn = new QRadioButton(dirWidget);
    d->m_cwRBtn->setText(tr("CW"));
    d->m_cwRBtn->setChecked(true);
    d->m_ccwRBtn = new QRadioButton(dirWidget);
    d->m_ccwRBtn->setText(tr("CCW"));
    d->m_ccwRBtn->setChecked(false);
    connect(d->m_cwRBtn,SIGNAL(clicked(bool)),this,SLOT(onRadioBtnClicked(bool)));
    connect(d->m_ccwRBtn,SIGNAL(clicked(bool)),this,SLOT(onRadioBtnClicked(bool)));
    hlayout->addWidget(d->m_cwRBtn);
    hlayout->addWidget(d->m_ccwRBtn);
    vlayout->addWidget(label);
    vlayout->addLayout(hlayout);
    dirWidget->setLayout(vlayout);
    d->m_dirItem->setWidget(dirWidget,true);
    d->m_scene->addItem(d->m_dirItem);
    d->m_widgetItemList.append(d->m_dirItem);
}

void GraphVelocity133::setBtnAutoTurningUiOn(bool on)
{
    Q_D(GraphVelocity133);
    d->m_btnAutoStart->setChecked(on);
}

void GraphVelocity133::setAutoTurningProgressBarValue(int v)
{
    Q_D(GraphVelocity133);
    d->m_bar->setValue(v);
}

void GraphVelocity133::setProgressBarVisible(bool visible)
{
    Q_D(GraphVelocity133);
    d->m_bar->setVisible(visible);
}
