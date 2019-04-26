#ifndef ADVUSERSTATION_H
#define ADVUSERSTATION_H

#include <QWidget>
#include "iadvuser.h"

namespace Ui {
class AdvUserStation;
}

class AdvUserStationPrivate;

class ADVUSERSHARED_EXPORT AdvUserStation : public IAdvUser
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AdvUserStation)

public:
    explicit AdvUserStation(const QString &advUserName, QWidget *parent = 0);
    ~AdvUserStation();
    QString nickName()Q_DECL_OVERRIDE;
    void uiInit() Q_DECL_OVERRIDE;
    void setSevList(const QList<SevDevice *> &list);

protected:
    bool advUserActive()Q_DECL_OVERRIDE;
    bool readAdv()Q_DECL_OVERRIDE;
    bool writeAdv()Q_DECL_OVERRIDE;
    void respondErrorExecute()Q_DECL_OVERRIDE;

private:
    Ui::AdvUserStation *ui;
private slots:
    void onComboBoxIndexChanged(int index);
    void onStationIdValueChanged(int value);
    void onBtnChecked(bool checked);
};

#endif // ADVUSERSTATION_H
