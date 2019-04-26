#include "uiioanalog.h"
#include "ui_uiioanalog.h"
#include "iuiwidget_p.h"
#include "igraphioanalog.h"
#include "sevdevice.h"

#include <QDebug>

class UiIOAnalogPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiIOAnalog)
public:
  UiIOAnalogPrivate();
  ~UiIOAnalogPrivate();

protected:
  IGraphIOAnalog *m_graphIOAnalog;
};

UiIOAnalogPrivate::UiIOAnalogPrivate():
  m_graphIOAnalog(NULL)
{

}
UiIOAnalogPrivate::~UiIOAnalogPrivate()
{
  qDebug()<<"UiIOAnalogPrivate destruct-->";
}

UiIOAnalog::UiIOAnalog(QWidget *parent) :
    IUiWidget(*(new UiIOAnalogPrivate),parent),ui(new Ui::UiIOAnalog)
{
    ui->setupUi(this);
}

UiIOAnalog::~UiIOAnalog()
{
    Q_D(UiIOAnalog);
    delete ui;
    delete d->m_graphIOAnalog;
}

void UiIOAnalog::accept(QWidget *w)
{
    Q_D(UiIOAnalog);
    ui->verticalLayout_2->addWidget(w);

    d->m_graphIOAnalog = dynamic_cast<IGraphIOAnalog *>(w);

    d->m_graphIOAnalog->visit(this);
    d->m_copyAll = false;
}

void UiIOAnalog::setUiActive(bool actived)
{
    Q_D(UiIOAnalog);
    if(actived)
    {
        bool ok;
        if (d->m_device->isOffline()) {
            ok = readOfflinePrm();
        } else {
            ok = readPageFLASH();
        }
        d->m_graphIOAnalog->setParaScale();
        if (ok) {
            d->m_graphIOAnalog->syncTreeDataToUiFace();
        }
    }
    if (d->m_device->isConnecting()) {
        d->m_graphIOAnalog->setTimerActive(actived);
    } else {
        d->m_graphIOAnalog->setTimerActive(false);
    }
}

bool UiIOAnalog::writePageFLASH()
{
    Q_D(UiIOAnalog);
    bool wOk=true;
    wOk = IUiWidget::writePageFLASH();
//    if (d->m_device->isOffline()) {
//        wOk = IUiWidget::writeOfflinePrm();
//    } else {
//        wOk = IUiWidget::writePageFLASH();
//    }
    if(wOk)
    {
        d->m_graphIOAnalog->syncTreeDataToUiFace();
    }
    return true;
}

bool UiIOAnalog::writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)
{
    Q_D(UiIOAnalog);
    bool wOk = true;
    wOk = IUiWidget::writePageFlashToOtherAxis(srcAxisInx, desAxisInx, tree);
    if (wOk)
    {
        d->m_graphIOAnalog->syncTreeDataToUiFace();
    }
    return true;
}

bool UiIOAnalog::hasConfigFunc()
{
    return false;
}

bool UiIOAnalog::hasSaveFunc()
{
    return true;
}

void UiIOAnalog::setContextAction()
{
    createActionSwitchView();
}

QStackedWidget *UiIOAnalog::getUiStackedWidget()
{
    return ui->stackedWidget;
}

QVBoxLayout *UiIOAnalog::getVBoxLayout()
{
    return ui->verticalLayout_tree;
}

void UiIOAnalog::setDefaultUi()
{
    setCurrentUiIndex(0);
}
