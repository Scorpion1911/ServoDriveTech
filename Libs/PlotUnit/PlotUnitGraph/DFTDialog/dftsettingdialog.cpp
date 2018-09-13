#include "dftsettingdialog.h"
#include "ui_dftsettingdialog.h"

DFTSettingDialog::DFTSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DFTSettingDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    connect(ui->btn_fft_apply, SIGNAL(clicked()), this, SLOT(onActionApplyClicked()));
    connect(ui->btn_fft_exit, SIGNAL(clicked()), this, SLOT(onActionExitClicked()));
}

DFTSettingDialog::~DFTSettingDialog()
{
    delete ui;
}

void DFTSettingDialog::onActionApplyClicked()
{
    QVector<double> limitList;
    limitList.append(ui->doubleSpinBox_fft_amp_xmax->value());
    limitList.append(ui->doubleSpinBox_fft_amp_xmin->value());
    limitList.append(ui->doubleSpinBox_fft_amp_ymax->value());
    limitList.append(ui->doubleSpinBox_fft_amp_ymin->value());
    limitList.append(ui->doubleSpinBox_fft_pha_xmax->value());
    limitList.append(ui->doubleSpinBox_fft_pha_xmin->value());
    limitList.append(ui->doubleSpinBox_fft_pha_ymax->value());
    limitList.append(ui->doubleSpinBox_fft_pha_ymin->value());

    emit axisRangeChanged(limitList);

    deleteLater();
}

void DFTSettingDialog::onActionExitClicked()
{
    deleteLater();
}
