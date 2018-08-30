#ifndef DFTSETTINGDIALOG_H
#define DFTSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class DFTSettingDialog;
}

class DFTSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DFTSettingDialog(QWidget *parent = 0);
    ~DFTSettingDialog();
signals:
    void axisRangeChanged(QVector<double>);
private slots:
    void onActionApplyClicked();
    void onActionExitClicked();
private:
    Ui::DFTSettingDialog *ui;
};

#endif // DFTSETTINGDIALOG_H
