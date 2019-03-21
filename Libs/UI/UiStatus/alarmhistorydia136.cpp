#include "alarmhistorydia136.h"
#include "ui_alarmhistorydia136.h"
#include "gtutils.h"
#include "QtTreeManager/qttreemanager.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>

#define ALARM_MAX_NUM       10

#define COLUMN_ALARM_CODE   0
#define COLUMN_ALARM_INFO   1

AlarmHistoryDia136::AlarmHistoryDia136(int axisInx, SevDevice* dev, QTreeWidgetItem *item, QWidget *parent) :
    m_dev(dev),
    m_axisInx(axisInx),
    m_treeItem(item),
    QDialog(parent),
    ui(new Ui::AlarmHistoryDia136)
{
    ui->setupUi(this);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    init();
}

AlarmHistoryDia136::~AlarmHistoryDia136()
{
    delete ui;
}

void AlarmHistoryDia136::init()
{
    qint16 index = 0;
    QList<qint32> alarmList;
    bool ok = m_dev->readAlarmHistoryIndex(m_axisInx, index);
    if (!ok) {
        return;
    }
    ok = m_dev->readAlarmHistoryList(m_axisInx, alarmList);
    if (!ok) {
        return;
    }
    int listIndex = 0;
    for (int i = 0; i < ALARM_MAX_NUM; i++) {
        listIndex = (index + ALARM_MAX_NUM) % ALARM_MAX_NUM;
        index--;
        ui->historyTable->insertRow(i);
        ui->historyTable->setItem(i, COLUMN_ALARM_CODE, new QTableWidgetItem(QString("0x%1").arg(alarmList.at(listIndex), 4, 16, QLatin1Char('0'))));
        QString binStr = BitItemHelper::decToBin32(QString::number(alarmList.at(listIndex)));
        int len = binStr.length();
        QString str = "";
        for (int j = 0; j < len; j++) {
            int mode = binStr.at(len - 1 - j).digitValue();
            if (mode == 1) {
                str = str + m_treeItem->child(j)->text(GT::COL_PAGE_TREE_INTRODUCT) + "\n";
            }
        }
        str.remove(str.length() - 1, 2);
        ui->historyTable->setItem(i, COLUMN_ALARM_INFO, new QTableWidgetItem(str));
    }
}
