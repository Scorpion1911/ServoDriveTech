#include "uimotor.h"
#include "ui_uimotor.h"
#include "iuiwidget_p.h"
#include "sevdevice.h"
#include "igraphmotor.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QMessageBox>
#include <QDebug>


class UiMotorPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiMotor)
public:
  UiMotorPrivate();
  ~UiMotorPrivate();

protected:
  IGraphMotor *m_graphMotor;
};

UiMotorPrivate::UiMotorPrivate():
  m_graphMotor(NULL)
{

}
UiMotorPrivate::~UiMotorPrivate()
{
  qDebug()<<"UiMotorPrivate destruct-->";
}

UiMotor::UiMotor(QWidget *parent):IUiWidget(*(new UiMotorPrivate),parent),ui(new Ui::UiMotor)
{
  ui->setupUi(this);

}
UiMotor::~UiMotor()
{
  Q_D(UiMotor);
  delete ui;
  delete d->m_graphMotor;
}

//!
//! \brief UiMotor::accept take owership of (w)
//! \param w
//!
void UiMotor::accept(QWidget *w)
{
  Q_D(UiMotor);
  ui->qmlHboxLayout->addWidget(w);
  d->m_graphMotor=dynamic_cast<IGraphMotor *>(w);
  d->m_graphMotor->visit(this);
  connect(d->m_graphMotor, SIGNAL(installMotor()), this, SLOT(onInstallMotor()));
  d->m_copyAll = false;
}
void UiMotor::setUiActive(bool actived)
{
  if(actived)
  {
    Q_D(UiMotor);
      bool ok;
      if (d->m_device->isOffline()) {
          ok = readOfflinePrm();
      } else {
          ok = readPageFLASH();
      }
    if(ok)
      d->m_graphMotor->syncTreeDataToUiFace();
  }
}
bool UiMotor::writePageFLASH()
{
  Q_D(UiMotor);
  bool wOk=true;
  wOk=IUiWidget::writePageFLASH();
  if(wOk)
  {
    d->m_graphMotor->syncTreeDataToUiFace();
    //还要加入关联参数处理
    //-to add
    d->m_device->imaxPrmAssociationActive(d->m_index.axisInx);
  }
  return true;
}

bool UiMotor::writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)
{
    Q_D(UiMotor);
    bool wOk=true;
    wOk = IUiWidget::writePageFlashToOtherAxis(srcAxisInx, desAxisInx, tree);
    if(wOk)
    {
      d->m_device->imaxPrmAssociationActive(desAxisInx);
    }
    return true;
}

bool UiMotor::hasConfigFunc()
{
  return false;
}

bool UiMotor::hasSaveFunc()
{
  return true;
}

void UiMotor::setContextAction()
{
  createActionSwitchView();
}

QStackedWidget *UiMotor::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}
QVBoxLayout *UiMotor::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiMotor::setDefaultUi()
{
    setCurrentUiIndex(0);
}

void UiMotor::onInstallMotor()
{
    bool ok = writePageFLASH();
    if (ok) {
        QMessageBox::information(0, tr("Info"), tr("Motor Install successfully!"));
    } else {
        QMessageBox::warning(0, tr("Info"), tr("Motor Install fails!"));
    }
}

