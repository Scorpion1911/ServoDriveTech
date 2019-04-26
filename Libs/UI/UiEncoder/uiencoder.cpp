#include "uiencoder.h"
#include "ui_uiencoder.h"
#include "iuiwidget_p.h"
#include "igraphencoder.h"
#include "sevdevice.h"
#include "gtutils.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QDebug>


class UiEncoderPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiEncoder)
public:
  UiEncoderPrivate();
  ~UiEncoderPrivate();
protected:
  IGraphEncoder *m_graphEncoder;

};
UiEncoderPrivate::UiEncoderPrivate()
{

}
UiEncoderPrivate::~UiEncoderPrivate()
{

}

UiEncoder::UiEncoder(QWidget *parent):IUiWidget(*(new UiEncoderPrivate),parent),ui(new Ui::UiEncoder)
{
  ui->setupUi(this);

}
UiEncoder::~UiEncoder()
{
  Q_D(UiEncoder);
  delete ui;
  delete d->m_graphEncoder;
}

bool UiEncoder::hasConfigFunc()
{
  return false;
}

bool UiEncoder::hasSaveFunc()
{
  return true;
}
void UiEncoder::accept(QWidget *w)
{
  Q_D(UiEncoder);
  ui->qmlHboxLayout->addWidget(w);
  d->m_graphEncoder=dynamic_cast<IGraphEncoder *>(w);
  d->m_graphEncoder->visit(this);
  d->m_copyAll = false;
}
void UiEncoder::setUiActive(bool actived)
{
  Q_D(UiEncoder);
  if(d->m_device->isConnecting())
  {
    if(actived)
    {
      //读一次FLASH的值
      //开启编码器刷新定时器及错误检查
      readPageFLASH();
      emit encActive();
    }
//    GTUtils::delayms(100);
    d->m_graphEncoder->startUpdateTimer(actived);
    qDebug()<<"TEST_OUT UiEncoder :d->m_graphEncoder->startUpdateTimer(actived)"<<actived;
  } else if (d->m_device->isOffline()){
      if (actived) {
          bool ok = readOfflinePrm();
          if (ok) {
            emit encActive();
          }
      }
  }
}

bool UiEncoder::writePageFLASH()
{
    Q_D(UiEncoder);
//    d->m_graphEncoder->onBtnEncConfigSaveClicked();
    return true;
}

void UiEncoder::setContextAction()
{
    createActionSwitchView();
}

bool UiEncoder::writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)
{
    Q_D(UiEncoder);
    bool wOk=true;
    wOk=IUiWidget::writePageFlashToOtherAxis(srcAxisInx, desAxisInx, tree);
    qint32 a;
    qint32 b;
    if (!wOk) {
        return false;
    }
    wOk = d->m_device->readGearPrm(srcAxisInx, a, b);
    if (!wOk) {
        return false;
    }
    wOk = d->m_device->writeGearPrm(desAxisInx, a, b);

    qint32 c;
    qint32 dd;
    if (!wOk) {
        return false;
    }
    wOk = d->m_device->readPulseGearPrm(srcAxisInx, c, dd);
    if (!wOk) {
        return false;
    }
    wOk = d->m_device->writeGearPrm(desAxisInx, c, dd);
    return wOk;
}

void UiEncoder::onDspReset()
{
  setUiActive(false);
}

QStackedWidget *UiEncoder::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}

QVBoxLayout *UiEncoder::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiEncoder::setDefaultUi()
{
  setCurrentUiIndex(0);
}

