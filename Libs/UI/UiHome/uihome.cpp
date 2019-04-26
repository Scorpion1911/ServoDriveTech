﻿#include "uihome.h"
#include "ui_uihome.h"

#include "igraphhome.h"
#include "iuiwidget_p.h"
#include "sevdevice.h"

#include <QDebug>

class UiHomePrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiHome)
public:
  UiHomePrivate();
  ~UiHomePrivate();

protected:
  IGraphHome *m_graphHome;
};

UiHomePrivate::UiHomePrivate():
  m_graphHome(NULL)
{

}
UiHomePrivate::~UiHomePrivate()
{
  qDebug()<<"UiHomePrivate destruct-->";
}

UiHome::UiHome(QWidget *parent) :
    IUiWidget(*(new UiHomePrivate),parent),ui(new Ui::UiHome)
{
    ui->setupUi(this);
}

UiHome::~UiHome()
{
    delete ui;
}

void UiHome::accept(QWidget *w)
{
    Q_D(UiHome);
    ui->qmlHboxLayout->addWidget(w);
    d->m_graphHome = dynamic_cast<IGraphHome *>(w);
    d->m_graphHome->visit(this);
    d->m_copyAll = false;
}

void UiHome::setUiActive(bool actived)
{
    if(actived)
    {
      Q_D(UiHome);
        bool ok;
        if (d->m_device->isOffline()) {
            ok = readOfflinePrm();
        } else {
            ok = readGenPageRAM();
        }
        if (ok) {
            d->m_graphHome->syncTreeDataToUiFace();
        }
    }
}

bool UiHome::writePageFLASH()
{
    Q_D(UiHome);
    d->m_graphHome->onBtnHomeConfigSaveClicked();
    bool ok = IUiWidget::writePageFLASH();
    if (ok) {
        d->m_graphHome->syncTreeDataToUiFace();
    }
    return true;
}

bool UiHome::hasConfigFunc()
{
    return false;
}

bool UiHome::hasSaveFunc()
{
    return true;
}

void UiHome::setContextAction()
{
    createActionSwitchView();
}

QStackedWidget *UiHome::getUiStackedWidget()
{
    return ui->stackedWidget;
}

QVBoxLayout *UiHome::getVBoxLayout()
{
    return ui->verticalLayout_tree;
}

void UiHome::setDefaultUi()
{
    setCurrentUiIndex(0);
}