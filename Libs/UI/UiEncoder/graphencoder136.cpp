#include "graphencoder136.h"
#include "ui_graphencoder136.h"
#include "igraphencoder_p.h"
#include "sdtglobaldef.h"
#include <qmath.h>

#include "EncConfig"
#include "idatabinding.h"
#include "encconfigbinding136.h"

#include "iuiwidget.h"
#include "sevdevice.h"

#include "qwt_dial_needle.h"
#include "uiencoder.h"

#include <QTimer>

#define KEY_NAME_POS          "gSevDrv.sev_obj.cur.rsv.pos"
#define KEY_NAME_POS_IN       "gSevDrv.sev_obj.cur.rsv.pos_in"
#define KEY_NAME_POS_OFFSET   "gSevDrv.sev_obj.cur.rsv.prm.pos_ofst_3"
#define KEY_NAME_PPN          "gSevDrv.sev_obj.cur.mot.PPN_1"
#define KEY_NAME_SEQ_DIR      "gSevDrv.sev_obj.cur.rsv.prm.seq_dir"
#define KEY_NAME_ENC_INFO     "gSevDrv.sev_obj.cur.pro.enc_info.all"
#define KEY_NAME_LOST         "gSevDrv.sev_obj.cur.rsv.prm.abs_type.all"
#define KEY_NAME_HOME_POS     "gSevDrv.sev_obj.pos.seq.prm.fix_pos"
#define KEY_ABS_POS           "FPGA.ENC.ABS_POS"

#define PRM_POS_OFFSET_INX 1
#define LINE_NUMBER_INDEX 2
#define FPGA_ABS_CFG_INDEX 3
#define PRM_SEQ_DIR_INX 4

#define INPUT_PULSE_REVERSE_MASK 0x0100
#define INPUT_PULSE_INDEX_MASK 0x3000
#define OUTPUT_PUSLE_REVERSE_MASK 0x0004
#define OUTPUT_PULSE_INDEX_MASK 0x0003

#define POW2_24             16777216

class GraphEncoder136Private:public IGraphEncoderPrivate
{
  Q_DECLARE_PUBLIC(GraphEncoder136)
public:
  GraphEncoder136Private(){}
  ~GraphEncoder136Private(){}
protected:

};

GraphEncoder136::GraphEncoder136(QWidget *parent) :
  IGraphEncoder(*(new GraphEncoder136Private),parent),
  ui(new Ui::GraphEncoder136)
{
  ui->setupUi(this);
  initDial(ui->Dial_encElectronic);
  initDial(ui->Dial_encMachine);
  ui->Dial_encElectronic->setTitle(tr("Electronic Angle"));
  ui->Dial_encMachine->setTitle(tr("Mechanical Angle"));
  ui->checkBox_pulseInputReverse->setVisible(false);

  setEncErrorUiEnable(false);

  setEncConfigUiEnable(false);

  ui->btn_encConfig->setCheckable(true);
  ui->spinBox_encLine->setVisible(true );
  ui->comboBox_encBitNum->setVisible(false);
  QStringList comList;
  comList<<"11"<<"13"<<"17"<<"20"<<"21"<<"22"<<"23"<<"24";
  ui->comboBox_encBitNum->addItems(comList);

  QStringList inputPulseList;
  inputPulseList<<tr("AB Pulse")<<tr("Direction Pulse")<<tr("Positive and Negative Pulse");
  ui->comboBox_pulseCmd->addItems(inputPulseList);

  QStringList outputPulseList;
  outputPulseList<<tr("AB Pulse")<<tr("Direction Pulse")<<tr("Positive and Negative Pulse");
  ui->comboBox_outputType->addItems(outputPulseList);

  ui->label_encLineNum->setText(tr("encoder lineNumber:"));
  ui->btn_encClearErr->setText(tr("clearAlarm"));
  ui->btn_encSavePhase->setEnabled(false);

  //-------------------初始化编码器配置设置框--------------------
  ui->enc_comboBox->addItem(tr("Enc SIN"));
  ui->enc_comboBox->addItem(tr("Enc Absolute"));
  ui->enc_comboBox->addItem(tr("Enc ROTATION"));
  ui->enc_comboBox->addItem(tr("Enc ABZ"));

  ui->label_encMsg->setText(tr("active after reset"));
  ui->label_encMsg->setVisible(false);

  ui->label_encSearchValue->setText("5");

  ui->checkBox_gear->setChecked(true);
  setGearAssociateUiEnable(false);

//  QStringList list;
//  list<<tr("0 DuoMoChuan")<<tr("1 NiKang")<<tr("2 Haidehan")<<tr("3 SanXie")<<tr("4 XiongXia")<<tr("5 AnChuan");
//  ui->listWidget_encAbsolute->addItems(list);

  connect(ui->enc_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onEncTypeChanged(int)));
  connect(ui->btn_encConfig,SIGNAL(clicked(bool)),this,SLOT(onBtnEncConfigClicked(bool)));
  connect(ui->btn_encSaveConfig,SIGNAL(clicked(bool)),this,SLOT(onBtnEncConfigSaveClicked()));
  connect(ui->btn_encSaveHome, SIGNAL(clicked(bool)), this, SLOT(onBtnSaveHomePosClicked()));
  connect(ui->listWidget_encItems,SIGNAL(currentRowChanged(int)),this,SLOT(onEncConfigListWidgetRowChanged(int)));
  connect(ui->btn_encClearErr,SIGNAL(clicked(bool)),this,SLOT(onBtnClearEcnAlarmClicked()));

  connect(ui->rbtn_encBit,SIGNAL(toggled(bool)),this,SLOT(onRadioBtnClicked()));
  connect(ui->rbtn_encLine,SIGNAL(toggled(bool)),this,SLOT(onRadioBtnClicked()));
  connect(ui->btn_encSearch,SIGNAL(clicked(bool)),this,SLOT(onBtnSearchPhaseClicked()));
  connect(ui->btn_encSavePhase,SIGNAL(clicked(bool)),this,SLOT(onBtnSavePhaseClicked()));
  connect(ui->spinBox_encLine, SIGNAL(valueChanged(int)), this, SLOT(onLineNumChanged(int)));

  connect(ui->checkBox_gear,SIGNAL(clicked(bool)),this,SLOT(onCheckBoxGearAssociationClicked(bool)));
}

GraphEncoder136::~GraphEncoder136()
{
  Q_D(GraphEncoder136);
  GT::deletePtrObject(d->m_encConfigManage);
  delete ui;
}
void GraphEncoder136::setCustomVisitActive(IUiWidget *uiWidget)
{
  Q_D(GraphEncoder136);
  d->m_iDataBinding=new EncConfigBinding136(this);
  qDebug()<<"d->m_iDataBinding=new EncConfigBinding136(this)";

  createSupportEncoderItem();
  qDebug()<<"createSupportEncoderItem()";

  UiEncoder *encUi=dynamic_cast<UiEncoder *>(uiWidget);
  connect(encUi,SIGNAL(encActive()),this,SLOT(onEncActive()));

}
void GraphEncoder136::setUiVersionName()
{
  Q_D(GraphEncoder136);
  d->m_versionName="V136";
}

void GraphEncoder136::setupDataMappings()
{

}
void GraphEncoder136::syncTreeDataToUiFace()
{
  Q_D(GraphEncoder136);
  qDebug()<<"TEST_OUT syncMultiTreeToUiData";
  d->m_iDataBinding->syncMultiTreeToUiData();//just sync tree data to encconfigdata

}
quint32 GraphEncoder136::getLineNumber()
{
  Q_D(GraphEncoder136);
  quint32 num=d->m_treeWidget->topLevelItem(LINE_NUMBER_INDEX)->text(GT::COL_PAGE_TREE_VALUE).toUInt();
  return num;
}
void GraphEncoder136::createSupportEncoderItem()
{
  Q_D(GraphEncoder136);
  d->m_encConfigManage=new EncConfigManage;

  d->m_encConfigManage->creatItemLists(4);

  //注意在这里addEncItem的顺序位置不能改变
  IEncConfigItem *encItem=new EncConfigDuoMoItem;
  encItem->createAttributeUi();
  //根据版本不同，还可以增加其它的设置
//  encItem->setWarningsString("aaa");
//  encItem->setLostOper(xxx);
//  encItem->setAlarmOper(xx);
  d->m_encConfigManage->addEncItem(1, encItem);

  encItem=new EncConfigNiKangItem;
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(1, encItem);

  encItem=new EncConfigHaidehanItem;
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(1, encItem);

  encItem=new EncConfigSanXieItem;
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(1, encItem);

  encItem=new EncConfigSongXiaItem;
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(1, encItem);

  encItem=new EncConfigYaskawaItem;
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(1, encItem);

  encItem = new EncConfigSinZ;
  connect(encItem, SIGNAL(lineNumChanged(int)), this, SLOT(receiveLineNum(int)));
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(0, encItem);

  encItem = new EncConfigSinHiperface;
  connect(encItem, SIGNAL(lineNumChanged(int)), this, SLOT(receiveLineNum(int)));
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(0, encItem);

  encItem = new EncConfigSinEndat;
  connect(encItem, SIGNAL(lineNumChanged(int)), this, SLOT(receiveLineNum(int)));
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(0, encItem);

  encItem = new EncConfigRotation;
  connect(encItem, SIGNAL(lineNumChanged(int)), this, SLOT(receiveLineNum(int)));
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(2, encItem);

  encItem = new EncConfigABZ;
  encItem->createAttributeUi();
  d->m_encConfigManage->addEncItem(3, encItem);

  ui->listWidget_encItems->addItems(d->m_encConfigManage->itemNames().at(ui->enc_comboBox->currentIndex()));

  ui->rbtn_encLine->setChecked(true);
}

void GraphEncoder136::onUpdateTimeOut()
{
  Q_D(GraphEncoder136);
  quint32 pos;
  quint32 posIn;
  quint32 posOfst;
  quint16 ppn;
  qint16 seqDir;
  quint16 encInfo;
  quint16 absType;
  quint32 absPos;
  pos=readPos(KEY_NAME_POS);
  //qDebug()<<"pos"<<pos;
  posIn=readPosInput(KEY_NAME_POS_IN);
  //qDebug()<<"posIn"<<posIn;
  posOfst=readPosOffset(KEY_NAME_POS_OFFSET);
  d->m_posOffset = posOfst;
  //qDebug()<<"posOfst"<<posOfst;
  ppn=readPPN(KEY_NAME_PPN);
  //qDebug()<<"ppn"<<ppn;
  seqDir=readSeqDir(KEY_NAME_SEQ_DIR);
  d->m_phaseDir = seqDir;
  //qDebug()<<"seqDir"<<seqDir;
  encInfo=readEncInfo(KEY_NAME_ENC_INFO);
  //qDebug()<<"encInfo errorcode"<<encInfo;
  absType=readErrLost(KEY_NAME_LOST);
 //qDebug()<<"absType lost"<<absType;

//  qDebug()<<"encoder axisSize:"<<d->m_dev->axisNum()<<"current axis"<<d->m_uiWidget->uiIndexs().axisInx<<"update ...";
//  var strPos=m_cmd.readCommand("gSevDrv.sev_obj.cur.rsv.pos");
//  pos.text=parseInt(strPos);
  ui->label_encReal->setText(QString::number(pos));
  ui->label_encOffset->setText(QString::number(posOfst));
  ui->label_encInput->setText(QString::number(posIn));
  ui->label_encPhase->setText(QString::number(seqDir));


//  var strPosIn=m_cmd.readCommand("gSevDrv.sev_obj.cur.rsv.pos_in");
//  posIn.text=parseInt(strPosIn);
//  var precision=parseInt(m_cmd.readCommand("gSevDrv.sev_obj.cur.rsv.prm.line_num_3"));
//  if(strPosIn!=="NULL")
//      gauge.value=360*parseInt(strPosIn)/precision;
  quint32 lineNumber=getLineNumber();
  //qDebug()<<"lineNumer"<<lineNumber;
  double machineValue=360*posIn/lineNumber;

  ui->Dial_encMachine->setValue(machineValue);

  double temp=(machineValue*ppn);
  int electronicValue=temp;
  electronicValue%=360;
  ui->Dial_encElectronic->setValue(electronicValue);

//  encInfo=0x10f0;
  showEncoderError(absType,encInfo);

  if (d->m_curEncConfigItem != NULL && d->m_curEncConfigItem->isNeedTimer()) {
      absPos = readAbsPos(KEY_ABS_POS);
      d->m_curEncConfigItem->setAbsPos(absPos);
  }

}

//!
//! \brief GraphEncoder136::onBtnEncConfigClicked 打开编码器设置对话框
//! \param checked
//!
void GraphEncoder136::onBtnEncConfigClicked(bool checked)
{
  setEncConfigUiEnable(checked);
  Q_D(GraphEncoder136);
  d->m_uiWidget->readPageFLASH();
  //读一次树数据,获取当前文件中的编码器配置信息
  initCurEncConfigItem();
  d->m_encConfigManage->setCurAttributeWidget(d->m_curEncConfigItem->attributeUi());
  d->m_iDataBinding->multiBind(static_cast<QObject*>(d->m_curEncConfigItem),d->m_treeWidget);
  d->m_iDataBinding->syncMultiTreeToUiData();//just sync tree data to encconfigdata

  updateEncConfigUiByCurrentConfigItem();

  ui->label_encMsg->setVisible(false);

  qDebug()<<"open encItemcount"<<d->m_encConfigManage->itemNames().count();

}
void GraphEncoder136::onRadioBtnClicked()
{
  if(ui->rbtn_encLine->isChecked())
  {
    ui->spinBox_encLine->setVisible(true);
    ui->comboBox_encBitNum->setVisible(false);
    ui->label_encLineNum->setText(tr("EnclineNumber:"));
    ui->spinBox_encLine->setValue(qPow(2,ui->comboBox_encBitNum->currentText().toUInt()));
  }
  else
  {
    ui->spinBox_encLine->setVisible(false);
    ui->comboBox_encBitNum->setVisible(true);
    ui->label_encLineNum->setText(tr("EncbitNumber:"));
  }
}
void GraphEncoder136::onBtnSearchPhaseClicked()
{
  Q_D(GraphEncoder136);
    ui->btn_encSearch->setEnabled(false);

  if(!d->m_dev->isConnecting())
    return ;
  qDebug()<<"search phase";
  if(d->m_dev->searchPhaseStart(d->m_uiWidget->uiIndexs().axisInx,ui->hSlider_encSearchPercent->value()))
    ui->btn_encSavePhase->setEnabled(true);

  ui->btn_encSearch->setEnabled(true);

//  static int test = 0;
//    d->m_posOffset = 100 + test;
//    d->m_phaseDir = test;
//    test ++;
}

void GraphEncoder136::onBtnSavePhaseClicked()
{
  Q_D(GraphEncoder136);
  d->m_treeWidget->topLevelItem(PRM_POS_OFFSET_INX)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(d->m_posOffset));
  d->m_treeWidget->topLevelItem(PRM_SEQ_DIR_INX)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(d->m_phaseDir));
  d->m_dev->writePageItemFlash(d->m_uiWidget->uiIndexs().axisInx,d->m_treeWidget->topLevelItem(PRM_POS_OFFSET_INX));
  d->m_dev->writePageItemFlash(d->m_uiWidget->uiIndexs().axisInx,d->m_treeWidget->topLevelItem(PRM_SEQ_DIR_INX));
}

void GraphEncoder136::onBtnSaveHomePosClicked()
{
    Q_D(GraphEncoder136);
    bool ok;
    quint64 value = d->m_dev->genCmdRead(KEY_NAME_HOME_POS, d->m_uiWidget->uiIndexs().axisInx, ok);
    ui->spinBox_homePos->setValue(value);
    qDebug()<<"read home pos ok"<<ok;
}

void GraphEncoder136::onEncConfigListWidgetRowChanged(int curRow)
{
  Q_D(GraphEncoder136);
    qDebug()<<"curRow"<<curRow;
    int boxIndex = ui->enc_comboBox->currentIndex();
  if(curRow<d->m_encConfigManage->itemNames().at(boxIndex).count() && curRow >= 0)
  {
    d->m_curEncConfigItem=d->m_encConfigManage->encItem(ui->enc_comboBox->currentIndex(), curRow);
    updateEncConfigUiByCurrentConfigItem();
  }
  qDebug()<<"current row="<<curRow;
}
void GraphEncoder136::readGearPrm()
{
  Q_D(GraphEncoder136);
  qint32 a = 1;
  qint32 b = 1;
  bool ret = d->m_dev->readGearPrm(d->m_uiWidget->uiIndexs().axisInx,a,b);
  if(ret)
  {
    ui->spinBox_gear_a->setValue(a);
    ui->spinBox_gear_b->setValue(b);
  }
}

void GraphEncoder136::onEncActive()
{
  qDebug()<<"onEncActive";
  onBtnEncConfigClicked(ui->btn_encConfig->isChecked());
  //initCurEncConfigItem();

  //读取电子齿轮参数
  readGearPrm();
}
void GraphEncoder136::onBtnClearEcnAlarmClicked()
{
  Q_D(GraphEncoder136);
  d->m_dev->genCmdWrite(KEY_NAME_ENC_INFO,0,d->m_uiWidget->uiIndexs().axisInx);
  qDebug()<<"clear enc alarm";
}

void GraphEncoder136::onCheckBoxGearAssociationClicked(bool checked)
{
    setGearAssociateUiEnable(!checked);
}

void GraphEncoder136::onEncTypeChanged(int index)
{
    Q_D(GraphEncoder136);
    ui->listWidget_encItems->clear();
    ui->listWidget_encItems->addItems(d->m_encConfigManage->itemNames().at(index));
    ui->listWidget_encItems->setCurrentRow(0);
}

void GraphEncoder136::receiveLineNum(int num)
{
    ui->spinBox_encLine->setValue(num);
}

void GraphEncoder136::onLineNumChanged(int num)
{
    int max = num / 16;
    if (max == 0) {
        max = 1;
    }
    ui->spinBox_resolution->setMaximum(max);
}

void GraphEncoder136::onBtnEncConfigSaveClicked()
{
  Q_D(GraphEncoder136);
  //quint8 inx=ui->listWidget_encItems->currentRow();

  //d->m_curEncConfigItem=d->m_encConfigManage->encItem(ui->enc_comboBox->currentIndex(), inx);
  if(d->m_curEncConfigItem!=NULL)
  {
    if(ui->rbtn_encBit->isChecked())
    {
      quint32 lineNum=ui->comboBox_encBitNum->currentText().toUInt();
      d->m_curEncConfigItem->setLineNumber(qPow(2,lineNum));

    }
    else
      d->m_curEncConfigItem->setLineNumber(ui->spinBox_encLine->value());
    qDebug()<<"lineNumer"<<d->m_curEncConfigItem->lineNumber();

    int revolusion = ui->spinBox_resolution->value();
    double shiftNum = log2(d->m_curEncConfigItem->lineNumber() / 16 / revolusion);
    d->m_curEncConfigItem->setShiftNum((quint16)shiftNum);

    d->m_curEncConfigItem->setPulseZCount(revolusion - 1);

    int index = ui->comboBox_outputType->currentIndex();
    bool outputReverse = ui->checkBox_outputReverse->isChecked();
    quint16 outReverseValue = 0;
    if (outputReverse) {
        outReverseValue = 1;
    }
    quint16 abModelValue = index + (outReverseValue << 2);
    d->m_curEncConfigItem->setPulseABModel(abModelValue);

    index = ui->comboBox_pulseCmd->currentIndex();
    bool reverse = ui->checkBox_pulseInputReverse->isChecked();
    quint16 reverseValue = 0;
    if (reverse) {
        reverseValue = 1;
    }
    quint16 cfgValue = (reverse << 8) + (index << 12);
    d->m_curEncConfigItem->setAufCfg(cfgValue);

    d->m_curEncConfigItem->execute();
    d->m_iDataBinding->multiBind(static_cast<QObject*>(d->m_curEncConfigItem),d->m_treeWidget);
    d->m_iDataBinding->syncMultiUiDataToTree();
  }
  d->m_uiWidget->writePageFLASH();

  //写电子齿轮参数
  qint32 a = ui->spinBox_gear_a->value();
  qint32 b = ui->spinBox_gear_b->value();
  if(ui->checkBox_gear->isChecked())
  {
    b = d->m_curEncConfigItem->lineNumber();
    a = POW2_24;
  }

  quint32 f = gdc(a,b);

  d->m_dev->writeGearPrm(d->m_uiWidget->uiIndexs().axisInx,a/f,b/f);

  qint32 c = ui->spinBox_resolution->value();
  qint32 dd = POW2_24;
  f = gdc(c, dd);
  d->m_dev->writePulseGearPrm(d->m_uiWidget->uiIndexs().axisInx,c/f,dd/f);

  if (d->m_dev->isConnecting() || d->m_dev->isOffline()) {
      ui->label_encMsg->setVisible(true);
  }

}

void GraphEncoder136::initDial(QwtDial *dial)
{
  QFontMetrics fm(qApp->font());
  QFont font;
  font.setPixelSize(fm.height()*1.5);
  dial->setFont(font);
  dial->setTracking( false );
  dial->setFocusPolicy( Qt::StrongFocus );

  QColor needleColor( Qt::red );

  dial->setOrigin( 90 );
  dial->setScaleArc( 0.0, 360.0 );
  dial->setScaleMaxMinor( 5 );
  dial->setScaleMaxMajor( 18 );
  dial->setScale( 0, 360.0 );

  needleColor = QColor( "Goldenrod" );


  QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
      QwtDialSimpleNeedle::Arrow, true, needleColor,
      QColor( Qt::gray ).light( 130 ) );

  dial->setNeedle( needle );

  const QColor base( QColor( Qt::darkGray ).dark( 110 ) );

  QPalette palette;
  palette.setColor( QPalette::Base, base );
  palette.setColor( QPalette::Window, base.dark( 110 ) );
  palette.setColor( QPalette::Mid, base.dark( 110 ) );
  palette.setColor( QPalette::Light, base.light( 170 ) );
  palette.setColor( QPalette::Dark, base.dark( 170 ) );
  palette.setColor( QPalette::Text, base.dark( 200 ).light( 800 ) );
  palette.setColor( QPalette::WindowText, base.dark( 200 ) );

  dial->setPalette( palette );
  dial->setLineWidth( 3 );
  dial->setFrameShadow( QwtDial::Raised );
}
void GraphEncoder136::setEncConfigUiEnable(bool en)
{
//  ui->comboBox_encBitNum->setVisible(en);
//  ui->label_encLineNum->setVisible(en);
//  ui->spinBox_encLine->setVisible(en);
//  ui->btn_encSaveConfig->setVisible(en);
//  ui->rbtn_encBit->setVisible(en);
//  ui->rbtn_encLine->setVisible(en);
//  ui->enc_comboBox->setVisible(en);
//  ui->groupBox_encLineSelect->setVisible(en);
  if (en) {
      ui->widget_config->setVisible(true);
      ui->widget_enc->setVisible(false);
  } else {
      ui->widget_config->setVisible(false);
      ui->widget_enc->setVisible(true);
  }
}
void GraphEncoder136::setEncErrorUiEnable(bool en)
{
  ui->listWidget_encErr->setVisible(en);
  ui->btn_encClearErr->setVisible(en);
}
void GraphEncoder136::updateEncConfigUiByCurrentConfigItem()
{
  Q_D(GraphEncoder136);
  if(d->m_curEncConfigItem!=NULL)
  {
    //更新编码器大类型
//    qDebug()<<"configdata"<<d->m_curEncConfigItem->encConfigData();
//    int enctypeInx=(int)d->m_curEncConfigItem->encType();
//    qDebug()<<"enctypeInx"<<enctypeInx;
//    int boxIndex = 0;
//    switch (enctypeInx) {
//    case 0:
//        boxIndex = 1;
//        break;
//    case 1:
//        boxIndex = 0;
//        break;
//    case 2:
//        boxIndex = 0;
//        break;
//    case 3:
//        boxIndex = 2;
//        break;
//    case 4:
//        boxIndex = 2;
//        break;
//    case 5:
//        boxIndex = 3;
//        break;
//    default:
//        boxIndex = 0;
//        break;
//    }
//    if(boxIndex<ui->enc_comboBox->count())
//      ui->enc_comboBox->setCurrentIndex(boxIndex);
//    else
//      ui->enc_comboBox->setCurrentIndex(0);

    //更新线数
    ui->spinBox_encLine->setValue(d->m_curEncConfigItem->lineNumber());
    //qDebug()<<"name 2"<<d->m_curEncConfigItem->objectName();

    //resolution
    int resolution = d->m_curEncConfigItem->lineNumber() / 16 / pow(2, d->m_curEncConfigItem->shiftNum());
    ui->spinBox_resolution->setValue(resolution);
    quint16 aufCfg = d->m_curEncConfigItem->aufCfg();
    if (((aufCfg & INPUT_PULSE_REVERSE_MASK) >> 8) == 1) {
        ui->checkBox_pulseInputReverse->setChecked(true);
    } else {
        ui->checkBox_pulseInputReverse->setChecked(false);
    }
    ui->comboBox_pulseCmd->setCurrentIndex((aufCfg & INPUT_PULSE_INDEX_MASK) >> 12);
    quint16 abModel = d->m_curEncConfigItem->pulseABModel();
    ui->comboBox_outputType->setCurrentIndex(abModel & OUTPUT_PULSE_INDEX_MASK);
    if (((abModel & OUTPUT_PUSLE_REVERSE_MASK) >> 2) == 1) {
        ui->checkBox_outputReverse->setChecked(true);
    } else {
        ui->checkBox_outputReverse->setChecked(false);
    }

    //更新编码器对应的特性UI
    QWidget *w=d->m_encConfigManage->curAttributeWidget();
    d->m_curEncConfigItem->updateAttributeUi();
    if(w!=NULL)
    {
      ui->vlayout_attributeWidget->removeWidget(w);
      w->hide();
    }
    ui->vlayout_attributeWidget->addWidget(d->m_curEncConfigItem->attributeUi());
    w=d->m_curEncConfigItem->attributeUi();
    d->m_encConfigManage->setCurAttributeWidget(w);
    ui->spinBox_encLine->setReadOnly(!d->m_curEncConfigItem->isLineNumEditable());
    ui->groupBox_encLineSelect->setEnabled(d->m_curEncConfigItem->isLineNumEditable());
    ui->comboBox_encBitNum->setEnabled(d->m_curEncConfigItem->isLineNumEditable());
    if(w!=NULL)
      w->show();
    qDebug()<<"ui->vlayout_encConfigUi->addWidget(d->m_curEncConfigItem->attributeUi()); ";
  }
}
void GraphEncoder136::showEncoderError(quint16 lost,quint16 encinfo)
{
  Q_D(GraphEncoder136);
  if(d->m_curEncConfigItem!=NULL)
  {
    QStringList errList;
    if(d->m_curEncConfigItem->hasLostError(lost))
    {
      //qDebug()<<"hasLostError";
      errList<<tr("Enc Lost");
    }
    else
    {
      if(d->m_curEncConfigItem->hasWarning(encinfo))
      {
        errList=d->m_curEncConfigItem->errorStrings(encinfo);
//        qDebug()<<errList;
      }
    }
    if(!errList.isEmpty())
    {
      ui->listWidget_encErr->clear();
      ui->listWidget_encErr->addItems(errList);
      setEncErrorUiEnable(true);
    }
    else
    {
      setEncErrorUiEnable(false);
    }
  }

}
void GraphEncoder136::initCurEncConfigItem()
{
  Q_D(GraphEncoder136);
  //绑编码器类型QList <->QTreeWidgetItem
  //根据QTreeWidgetItem 更新QList
  //初始化当前encConfigItem
  //d->m_iDataBinding->bind(ui->listWidget_encItems,d->m_treeWidget->topLevelItem(FPGA_ABS_CFG_INDEX));//FPGA.prm.ABS_ENC_CFG.all
  //d->m_iDataBinding->syncTreeItemToUiData();
  //d->m_curEncConfigItem=d->m_encConfigManage->encItem(ui->enc_comboBox->currentIndex(), ui->listWidget_encItems->currentRow());
//  d->m_curEncConfigItem = new IEncConfigItem;
//  d->m_curEncConfigItem->setEncType(0);
//  d->m_curEncConfigItem->createAttributeUi();
    qDebug()<<"dd";
    int enctypeInx = d->m_treeWidget->topLevelItem(5)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
    qDebug()<<"enctypeInx"<<enctypeInx;
    int boxIndex = 0;
    int listIndex = 0;
    switch (enctypeInx) {
    case 0:
        boxIndex = 1;
        break;
    case 1:
        boxIndex = 0;
        break;
    case 2:
        boxIndex = 0;
        break;
    case 3:
        boxIndex = 2;
        break;
    case 4:
        boxIndex = 2;
        break;
    case 5:
        boxIndex = 3;
        break;
    default:
        boxIndex = 0;
        break;
    }
    if(boxIndex<ui->enc_comboBox->count())
      ui->enc_comboBox->setCurrentIndex(boxIndex);
    else
      ui->enc_comboBox->setCurrentIndex(0);
    quint16 protocolIndex = d->m_treeWidget->topLevelItem(3)->text(GT::COL_PAGE_TREE_VALUE).toDouble();
    protocolIndex &= 0x000f;
    switch (boxIndex) {
    case 0:
        switch (protocolIndex) {
        case 15:
            listIndex = 0;
            break;
        case 2:
            listIndex = 2;
            break;
        case 8:
            listIndex = 1;
            break;
        }
        break;
    case 1:
        listIndex = protocolIndex;
        break;
    case 2:
        listIndex = 0;
        break;
    case 3:
        listIndex = 0;
        break;
    default:
        listIndex = 0;
        break;
    }
    ui->listWidget_encItems->setCurrentRow(listIndex);

    d->m_curEncConfigItem=d->m_encConfigManage->encItem(boxIndex, listIndex);


  readGearPrm();
}

void GraphEncoder136::setGearAssociateUiEnable(bool en)
{
  ui->label_gear_a->setVisible(en);
  ui->label_gear_b->setVisible(en);
  ui->spinBox_gear_a->setVisible(en);
  ui->spinBox_gear_b->setVisible(en);
}

quint32 GraphEncoder136::gdc(qint32 a, qint32 b)
{
  quint32 ret ;
  if(a > b)
    ret = b;
  else
    ret = a;
  while((a%ret!=0)||(b%ret!=0))
  {
    ret --;
    //qDebug()<<"ret = "<<ret;
  }
  return ret;
}
