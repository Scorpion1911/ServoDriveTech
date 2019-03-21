#ifndef ADVUSERFIRMWARESEGMENTFLASH_H
#define ADVUSERFIRMWARESEGMENTFLASH_H

#include <QWidget>
#include "iadvuser.h"

namespace Ui {
class AdvUserFirmwareSegmentFlash;
}

class AdvUserFirmwareSegmentFlashPrivate;

class ADVUSERSHARED_EXPORT AdvUserFirmwareSegmentFlash : public IAdvUser
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AdvUserFirmwareSegmentFlash)

public:
    explicit AdvUserFirmwareSegmentFlash(const QString &advUserName, QWidget *parent = 0);
    ~AdvUserFirmwareSegmentFlash();
    QString nickName()Q_DECL_OVERRIDE;
    void uiInit() Q_DECL_OVERRIDE;
    void setSevList(const QList<SevDevice *> &list);
signals:
    void startDownload(bool en);

protected:
    bool advUserActive()Q_DECL_OVERRIDE;
    bool readAdv()Q_DECL_OVERRIDE;
    bool writeAdv()Q_DECL_OVERRIDE;
    void respondErrorExecute()Q_DECL_OVERRIDE;

    static void updateProgressBar(void *arg,qint16 *value);
    bool firmwareFlashCheck();

private slots:
    void onActionComboBoxIndexChange(int index);
    void onActionDSPToolBtnClicked();
    void onActionFPGAToolBtnClicked();
    void onActionFLASHToolBtnClicked();
    void onActionDSPFlashBtnClicked();
    void onActionFPGAFlashBtnClicked();
    void onActionFLASHFlashBtnClicked();

private:
    Ui::AdvUserFirmwareSegmentFlash *ui;
};

#endif // ADVUSERFIRMWARESEGMENTFLASH_H
