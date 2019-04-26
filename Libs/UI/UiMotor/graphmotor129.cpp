#include "graphmotor129.h"
#include "ui_graphmotor129.h"
#include "igraphmotor_p.h"
#include "iuiwidget.h"
#include "uimotor.h"
#include "sevdevice.h"
#include "boxitemmapping.h"
#include "motordbui.h"
#include "Option"

#include <QDebug>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>

class GraphMotor129Private:public IGraphMotorPrivate
{
  Q_DECLARE_PUBLIC(GraphMotor129)
public:
  GraphMotor129Private(){}
  ~GraphMotor129Private(){}
};

GraphMotor129::GraphMotor129(QWidget *parent) :
  IGraphMotor(*(new GraphMotor129Private),parent),
  ui(new Ui::GraphMotor129)
{
  ui->setupUi(this);
  connect(ui->tbtn_motorDB, SIGNAL(clicked()), this, SLOT(onMotorInstallationBtnClicked()));
  QWidget *w = ui->stackedWidget->widget(1);
  ui->stackedWidget->removeWidget(w);
  delete w;
  MotorDBUi *dbUi = new MotorDBUi;
  ui->stackedWidget->insertWidget(1, dbUi);
  connect(dbUi, SIGNAL(returnClicked()), this, SLOT(onReturnBtnClicked()));
  connect(dbUi, SIGNAL(onMotorInstall(QStringList)), this, SLOT(onInstallMotorReceived(QStringList)));

}

GraphMotor129::~GraphMotor129()
{
  delete ui;
}
void GraphMotor129::syncTreeDataToUiFace()
{
  //将树的数据全更新到用户界面上
  Q_D(GraphMotor129);
  d->m_mapping->syncAllItem2BoxText();
}

void GraphMotor129::setCustomVisitActive(IUiWidget *uiWidget)
{
  //Q_D(GraphMotor129);
  Q_UNUSED(uiWidget);
  this->ui->dspinBox_maxVoltage->setEnabled(false);

}
void GraphMotor129::setUiVersionName()
{
  Q_D(GraphMotor129);
  d->m_versionName="V129";
}

void GraphMotor129::setupDataMappings()
{
  Q_D(GraphMotor129);

  QList<QDoubleSpinBox *>bList;
  //bList的先后位置不能变
  bList<<ui->dspinBox_iRat\
      <<ui->dspinBox_iMax\
     <<ui->dspinBox_vRat\
    <<ui->dspinBox_vOver\
   <<ui->dspinBox_vMax\
  <<ui->dspinBox_torque\
  <<ui->dspinBox_pole\
  <<ui->dspinBox_maxVoltage\
  <<ui->dspinBox_phim\
  <<ui->dspinBox_ldm\
  <<ui->dspinBox_lqm\
  <<ui->dspinBox_rm\
  <<ui->dspinBox_Jm\
  <<ui->dspinBox_JmPercent\
  <<ui->dspinBox_fcoe;

  if(bList.count()<=d->m_treeWidget->topLevelItemCount())
  {
    for(int i=0;i<bList.count();i++)
    {
      d->m_mapping->insertBox2Item(bList.at(i),d->m_treeWidget->topLevelItem(i));
      d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(i), bList.at(i));
    }
  }
}


void GraphMotor129::onDoubleSpinBoxFocusOut()
{
  Q_D(GraphMotor129);
  QDoubleSpinBox *box=qobject_cast<QDoubleSpinBox *>(sender());
  QTreeWidgetItem *item=d->m_mapping->item(box);
  d->m_mapping->syncItem2BoxText(item);
  qDebug()<<"focus out"<<box;
}

void GraphMotor129::onMotorInstallationBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void GraphMotor129::onReturnBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void GraphMotor129::onInstallMotorReceived(const QStringList &paraList)
{
    Q_D(GraphMotor129);
    if (!d->m_dev->isConnecting() && !d->m_dev->isOffline()) {
        QMessageBox::information(0, tr("Warning"), tr("Please connect the device!"));
        return;
    }
    //qDebug()<<"length"<<paraList.length();
    ui->dspinBox_iMax->setValue(paraList.at(0).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_iMax);

    ui->dspinBox_iRat->setValue(paraList.at(1).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_iRat);

    ui->dspinBox_vRat->setValue(paraList.at(2).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_vRat);

    ui->dspinBox_vOver->setValue(paraList.at(3).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_vOver);

    ui->dspinBox_vMax->setValue(paraList.at(4).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_vMax);

    ui->dspinBox_rm->setValue(paraList.at(5).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_rm);

    ui->dspinBox_ldm->setValue(paraList.at(6).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_ldm);

    ui->dspinBox_lqm->setValue(paraList.at(7).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_lqm);

    ui->dspinBox_Jm->setValue(paraList.at(8).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_Jm);

    ui->dspinBox_JmPercent->setValue(paraList.at(9).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_JmPercent);

    ui->dspinBox_fcoe->setValue(paraList.at(10).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_fcoe);

    ui->dspinBox_pole->setValue(paraList.at(11).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_pole);

    ui->dspinBox_torque->setValue(paraList.at(12).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_torque);

    ui->dspinBox_phim->setValue(paraList.at(13).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_phim);

    ui->dspinBox_maxVoltage->setValue(paraList.at(14).toDouble());
    d->m_mapping->syncBoxText2Item(ui->dspinBox_maxVoltage);

    emit installMotor();
}

