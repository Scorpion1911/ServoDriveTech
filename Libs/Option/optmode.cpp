#include "optmode.h"
#include "ui_optmode.h"
#include "iopt_p.h"

#include <QDebug>

class OptModePrivate: public IOptPrivate {
    Q_DECLARE_PUBLIC(OptMode)
    public:
      OptModePrivate();
      ~OptModePrivate();
      bool m_isOffline;
};

OptModePrivate::OptModePrivate()
{

}

OptModePrivate::~OptModePrivate()
{

}

OptMode::OptMode(const QString &optName, QWidget *parent) :
    IOpt(optName, *new OptModePrivate,parent),
    ui(new Ui::OptMode)
{
    Q_D(OptMode);
    ui->setupUi(this);
    d->m_isOffline = false;
    connect(ui->btn_mode_offline, SIGNAL(clicked()), this, SLOT(onActionBtnOfflineClicked()));
    connect(ui->btn_mode_online, SIGNAL(clicked()), this, SLOT(onActionBtnOnlineClicked()));
}

OptMode::~OptMode()
{
    delete ui;
}

void OptMode::uiInit()
{
    Q_D(OptMode);
    ui->btn_mode_offline->setChecked(d->m_isOffline);
    ui->btn_mode_online->setChecked(!d->m_isOffline);
}

QString OptMode::nickName()
{
    return tr("Mode");
}

bool OptMode::isOffline()
{
    Q_D(OptMode);
    return d->m_isOffline;
}

void OptMode::setOfflineMode(bool mode)
{
    Q_D(OptMode);
    d->m_isOffline = mode;
    if (mode) {
        ui->btn_mode_offline->setChecked(true);
        ui->btn_mode_online->setChecked(false);
    } else {
        ui->btn_mode_offline->setChecked(false);
        ui->btn_mode_online->setChecked(true);
    }
    emit softwareModeChanged(d->m_isOffline);
}

bool OptMode::optActive()
{
    Q_D(OptMode);
    emit softwareModeChanged(d->m_isOffline);
    return true;
}

bool OptMode::readOpt()
{
    Q_D(OptMode);
    d->m_isOffline = data("mode", "offlineMode", false).toBool();
    return true;
}

bool OptMode::writeOpt()
{
    //Q_D(OptMode);
    //saveData("mode", "offlineMode", d->m_isOffline);
    return true;
}

void OptMode::respondErrorExecute()
{

}

void OptMode::onActionBtnOfflineClicked()
{
    Q_D(OptMode);
    if (ui->btn_mode_offline->isChecked()) {
        d->m_isOffline = true;
    }
    d->m_isModify = true;
}

void OptMode::onActionBtnOnlineClicked()
{
    Q_D(OptMode);
    if (ui->btn_mode_online->isChecked()) {
        d->m_isOffline = false;
    }
    d->m_isModify = true;
}
