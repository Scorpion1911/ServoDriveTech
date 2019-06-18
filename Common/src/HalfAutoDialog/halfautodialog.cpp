#include "halfautodialog.h"
#include "ui_halfautodialog.h"
#include "deviceconfig.h"

HalfAutoDialog::HalfAutoDialog(QList<DeviceConfig*> &srcList, QList<DeviceConfig*> &desList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HalfAutoDialog)
{
    ui->setupUi(this);
}

HalfAutoDialog::~HalfAutoDialog()
{
    delete ui;
}
