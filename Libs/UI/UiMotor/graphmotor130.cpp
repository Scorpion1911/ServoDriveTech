#include "graphmotor130.h"
#include "ui_graphmotor130.h"
#include "igraphmotor_p.h"
#include "iuiwidget.h"
#include "uimotor.h"
#include "sevdevice.h"
#include "boxitemmapping.h"
#include "Option"
#include "motordbui.h"

#include <QDebug>
#include <QTreeWidget>
#include <QMessageBox>
#include <QKeyEvent>

class GraphMotor130Private:public IGraphMotorPrivate
{
  Q_DECLARE_PUBLIC(GraphMotor130)
public:
  GraphMotor130Private(){}
  ~GraphMotor130Private(){}
};

GraphMotor130::GraphMotor130(QWidget *parent) :
  IGraphMotor(*(new GraphMotor130Private),parent),
  ui(new Ui::GraphMotor130)
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

GraphMotor130::~GraphMotor130()
{
  delete ui;
}
void GraphMotor130::syncTreeDataToUiFace()
{
  //将树的数据全更新到用户界面上
  Q_D(GraphMotor130);
  d->m_mapping->syncAllItem2BoxText();
}

void GraphMotor130::setCustomVisitActive(IUiWidget *uiWidget)
{
  Q_D(GraphMotor130);
  Q_UNUSED(uiWidget);
  this->ui->dspinBox_maxVoltage->setEnabled(false);
//    connect(ui->tbtn_motorDB, SIGNAL(clicked()), this, SLOT(onMotorInstallationBtnClicked()));
//    QWidget *w = ui->stackedWidget->widget(1);
//    ui->stackedWidget->removeWidget(w);
//    delete w;
//    MotorDBUi *dbUi = new MotorDBUi;
//    ui->stackedWidget->insertWidget(1, dbUi);
}
void GraphMotor130::setUiVersionName()
{
  Q_D(GraphMotor130);
  d->m_versionName="V130";
}

void GraphMotor130::setupDataMappings()
{
  Q_D(GraphMotor130);

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
      d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(i),bList.at(i));
    }
  }
}


void GraphMotor130::onDoubleSpinBoxFocusOut()
{
  Q_D(GraphMotor130);
  QDoubleSpinBox *box=qobject_cast<QDoubleSpinBox *>(sender());
  QTreeWidgetItem *item=d->m_mapping->item(box);
  d->m_mapping->syncItem2BoxText(item);
  qDebug()<<"focus out"<<box;
}

void GraphMotor130::onMotorInstallationBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void GraphMotor130::onReturnBtnClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void GraphMotor130::onInstallMotorReceived(const QStringList &paraList)
{
    Q_D(GraphMotor130);
    if (!d->m_dev->isConnecting() && !d->m_dev->isOffline()) {
        QMessageBox::information(0, tr("Warning"), tr("Please connect the device!"));
        return;
    }
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

