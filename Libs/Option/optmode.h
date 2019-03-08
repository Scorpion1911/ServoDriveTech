#ifndef OPTMODE_H
#define OPTMODE_H

#include <QWidget>
#include "iopt.h"

namespace Ui {
class OptMode;
}

class OptModePrivate;

class OPTIONSHARED_EXPORT OptMode : public IOpt
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(OptMode)

public:
    explicit OptMode(const QString &optName, QWidget *parent = 0);
    ~OptMode();
    void uiInit() Q_DECL_OVERRIDE;
    QString nickName()Q_DECL_OVERRIDE;
    bool isOffline();
    void setOfflineMode(bool mode);
signals:
    void softwareModeChanged(bool mode);
protected:
    bool optActive() Q_DECL_OVERRIDE;
    bool readOpt() Q_DECL_OVERRIDE;
    bool writeOpt() Q_DECL_OVERRIDE;
    void respondErrorExecute() Q_DECL_OVERRIDE;

private:
    Ui::OptMode *ui;
private slots:
    void onActionBtnOfflineClicked();
    void onActionBtnOnlineClicked();
};

#endif // OPTMODE_H
