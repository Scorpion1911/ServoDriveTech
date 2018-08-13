#ifndef MOTORDBUi_H
#define MOTORDBUi_H

#include <QWidget>
#include <QMap>

class MotorDBManager;
class QTableWidgetItem;

namespace Ui {
class MotorDBUi;
}

class MotorDBUi : public QWidget
{
    Q_OBJECT

public:
    explicit MotorDBUi(QWidget *parent = 0);
    ~MotorDBUi();
signals:
    void returnClicked();
    void onMotorInstall(const QStringList &list);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private:
    void initUi();
    void initMap();
    void createConnections();
private slots:
    void onCompanyRowChanged(int index);
    void onMotorRowChanged(int index);
    void onActionNewBtnClicked();
    void onActionRemoveBtnClicked();
    void onActionSaveBtnClicked();
    void onActionAddBtnClicked();
    void onActionInstallBtnClicked();
    void onActionOkBtnClicked();
    void onActionCancelBtnClicked();
    void onItemDoubleClicked(QTableWidgetItem *item);
    void onItemEditingFinished();

private:
    Ui::MotorDBUi *ui;
    MotorDBManager *m_dbManager;
    QMap<QString, QString> m_map;
    QMap<QString, QString> m_unitMap;
    QStringList m_paraNameList;
    QList<int> m_companyIdList;
    QList<int> m_motorIdList;
    QStringList m_motorParaList;
    QString m_originText;
    bool m_isEditing;
    QTableWidgetItem *m_editedItem;
    bool m_isEditValid;
};

#endif // MOTORDBUi_H
