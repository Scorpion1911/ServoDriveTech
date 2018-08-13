#ifndef FIRMWARESEGMENTFLASHDIALOG_H
#define FIRMWARESEGMENTFLASHDIALOG_H

#include <QDialog>
#include "sevdevice.h"
#include "ServoDriverComDef.h"
#include "ServoDriverComDll.h"
#include <QString>
#include <QStringList>

namespace Ui {
class FirmwareSegmentFlashDialog;
}

class FirmwareSegmentFlashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FirmwareSegmentFlashDialog(QWidget *parent = 0);
    ~FirmwareSegmentFlashDialog();



private:
    void uiInit();
    Ui::FirmwareSegmentFlashDialog *ui;
//    QList<SevDevice *> m_devList;

    static void updateProgressBar(void *arg, qint16 *value);

    QString m_hexFilePath;
    QString m_rpdFilePath;
    QStringList m_xmlFilePaths;

    int16 m_axisNum;


private slots:
    void onActionConnectClicked();
    void onActionDisconnectClicked();

    void onActionDSPToolBtnClicked();
    void onActionFPGAToolBtnClicked();
    void onActionFLASHToolBtnClicked();
    void onActionDSPFlashBtnClicked();
    void onActionFPGAFlashBtnClicked();
    void onActionFLASHFlashBtnClicked();
};

#endif // FIRMWARESEGMENTFLASHDIALOG_H
