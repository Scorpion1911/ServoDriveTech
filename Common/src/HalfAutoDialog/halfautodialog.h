#ifndef HALFAUTODIALOG_H
#define HALFAUTODIALOG_H

#include <QDialog>
#include "deviceconfig.h"

namespace Ui {
class HalfAutoDialog;
}

class HalfAutoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HalfAutoDialog(QList<DeviceConfig *> &srcList, QList<DeviceConfig *> &desList, QWidget *parent = 0);
    ~HalfAutoDialog();

private:
    Ui::HalfAutoDialog *ui;
};

#endif // HALFAUTODIALOG_H
