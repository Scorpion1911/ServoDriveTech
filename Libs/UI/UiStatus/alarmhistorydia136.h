#ifndef ALARMHISTORYDIA136_H
#define ALARMHISTORYDIA136_H

#include <QDialog>

namespace Ui {
class AlarmHistoryDia136;
}

class SevDevice;
class QTreeWidget;
class QTreeWidgetItem;

class AlarmHistoryDia136 : public QDialog
{
    Q_OBJECT

public:
    explicit AlarmHistoryDia136(int axisInx, SevDevice* dev, QTreeWidgetItem* item, QWidget *parent = 0);
    ~AlarmHistoryDia136();
private:
    void init();
private:
    Ui::AlarmHistoryDia136 *ui;
    SevDevice* m_dev;
    int m_axisInx;
    QTreeWidgetItem* m_treeItem;
};

#endif // ALARMHISTORYDIA136_H
