#include "uiioconf.h"
#include "ui_uiioconf.h"
#include "iuiwidget_p.h"
#include "sevdevice.h"
#include "igraphioconf.h"

#include <QQuickWidget>
#include <QDebug>


class UiIOConfPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiIOConf)
public:
  UiIOConfPrivate();
  ~UiIOConfPrivate();
protected:
  IGraphIOConf *m_graphIOConf;
};
UiIOConfPrivate::UiIOConfPrivate():
    m_graphIOConf(NULL)
{

}
UiIOConfPrivate::~UiIOConfPrivate()
{
    delete m_graphIOConf;
    qDebug()<<"UiIOConfPrivate destruct-->";
}

UiIOConf::UiIOConf(QWidget *parent):IUiWidget(*(new UiIOConfPrivate),parent),ui(new Ui::UiIOConf)
{
  ui->setupUi(this);
}
UiIOConf::~UiIOConf()
{
  delete ui;
}

void UiIOConf::accept(QWidget *w)
{
    Q_D(UiIOConf);
    ui->horizontalLayout->addWidget(w);
    d->m_graphIOConf = dynamic_cast<IGraphIOConf *>(w);
    d->m_graphIOConf->visit(this);
    d->m_graphIOConf->initUi();
    d->m_copyAll = false;
}

void UiIOConf::setUiActive(bool actived)
{
    Q_D(UiIOConf);
    if(actived)
    {
        bool ok;
        if (d->m_device->isOffline()) {
            ok = readOfflinePrm();
        } else {
            ok = readPageFLASH();
        }
      if (ok) {
          d->m_graphIOConf->syncTreeDataToUiFace();
      }
    }
    if (d->m_device->isConnecting()) {
        d->m_graphIOConf->setTimerActive(actived);
    } else {
        d->m_graphIOConf->setTimerActive(false);
    }
}

bool UiIOConf::writePageFLASH()
{
    Q_D(UiIOConf);
    bool wOk=true;
    d->m_graphIOConf->syncUiToTree();
    wOk = IUiWidget::writePageFLASH();
//    if (d->m_device->isOffline()) {
//        wOk = IUiWidget::writeOfflinePrm();
//    } else {
//        wOk = IUiWidget::writePageFLASH();
//    }

    if(wOk)
    {
        d->m_graphIOConf->syncTreeDataToUiFace();
    }
    return true;
}

bool UiIOConf::writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)
{
    Q_D(UiIOConf);
    bool wOk = true;
    wOk = IUiWidget::writePageFlashToOtherAxis(srcAxisInx, desAxisInx, tree);
    if(wOk)
    {
        d->m_graphIOConf->syncTreeDataToUiFace();
    }
    return true;
}

bool UiIOConf::hasConfigFunc()
{
    return false;
}

bool UiIOConf::hasSaveFunc()
{
    return true;
}

void UiIOConf::setContextAction()
{

}

QStackedWidget *UiIOConf::getUiStackedWidget()
{
    return ui->stackedWidget;
}

QVBoxLayout *UiIOConf::getVBoxLayout()
{
    return ui->verticalLayout_tree;
}

void UiIOConf::setDefaultUi()
{
    setCurrentUiIndex(0);
}
