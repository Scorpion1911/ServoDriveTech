#include "bititemhelper.h"
#include "gtutils.h"
#include "sdtglobaldef.h"
#include "rnnet.h"
#include "pcdebug.h"

#include <QTreeWidget>
#include <QSettings>
#include <QDebug>

#define TABLE_INI "ramMap.ini"

using namespace ComDriver;

BitItemHelper::BitItemHelper(QObject *parent): QObject(parent)
{

}

void BitItemHelper::assign16Bits(QTreeWidgetItem *item, uint16_t value)
{
    quint16 temp = 0;
//    qDebug()<<"count"<<item->childCount();
//    qDebug()<<"value"<<value;
    for (int i = 0; i < item->childCount(); i++) {
        int start = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITSTART).toInt();
        int length = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt();
        quint16 opera = getOperationValue(start, length);
        temp = value & opera;
        temp >>= start;
        //qDebug()<<i<<temp;
        item->child(i)->setText(GT::COL_FLASH_RAM_TREE_VALUE, QString::number(temp));
    }
}

void BitItemHelper::assign32Bits(QTreeWidgetItem *item, uint32_t value)
{
    quint32 temp = 0;
//    qDebug()<<"count"<<item->childCount();
//    qDebug()<<"value"<<value;
    for (int i = 0; i < item->childCount(); i++) {
        int start = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITSTART).toInt();
        int length = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt();
        quint32 opera = getOperationValue(start, length);
        temp = value & opera;
        temp >>= start;
        //qDebug()<<i<<temp;
        item->child(i)->setText(GT::COL_FLASH_RAM_TREE_VALUE, QString::number(temp));
    }
}

void BitItemHelper::assign64Bits(QTreeWidgetItem *item, uint64_t value)
{
    quint64 temp = 0;
//    qDebug()<<"count"<<item->childCount();
//    qDebug()<<"value"<<value;
    for (int i = 0; i < item->childCount(); i++) {
        int start = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITSTART).toInt();
        int length = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt();
        quint64 opera = getOperationValue(start, length);
        temp = value & opera;
        temp >>= start;
        //qDebug()<<i<<temp;
        item->child(i)->setText(GT::COL_FLASH_RAM_TREE_VALUE, QString::number(temp));
    }
}

quint64 BitItemHelper::getOperationValue(int start, int length)
{
    quint64 temp = 0;
    for (int i = 0; i < length; i++) {
        temp <<= 1;
        temp += 1;
    }
    temp <<= start;
    return temp;
}

bool BitItemHelper::isTargetItem(QTreeWidgetItem *item, const QString &str, int index)
{
    if (item != NULL) {
        QStringList list = item->text(GT::COL_FLASH_RAM_TREE_NAME).split(".");
        if (str.compare(list.at(index)) == 0) {
            return true;
        }
    }
    return false;
}

QString BitItemHelper::decToBin16(const QString &decStr)
{
    QString binStr = "0000000000000000";
    bool ok;
    int value = decStr.toInt(&ok);
    if (!ok) {
        return "";
    }
    int count = binStr.length() - 1;
    while (value != 0) {
        int mode = value % 2;
        value = value / 2;
        binStr.replace(count--, 1, QString::number(mode));
    }
    return binStr;
}

QString BitItemHelper::decToBin32(const QString &decStr)
{
    QString binStr = "00000000000000000000000000000000";
    bool ok;
    int value = decStr.toInt(&ok);
    if (!ok) {
        return "";
    }
    int count = binStr.length() - 1;
    while (value != 0) {
        int mode = value % 2;
        value = value / 2;
        binStr.replace(count--, 1, QString::number(mode));
    }
    return binStr;
}

QString BitItemHelper::binToDec(const QString &binStr)
{
    int decValue = 0;
    int len = binStr.length();
    for (int i = 0; i < len; i++) {
        int mode = binStr.at(len - 1 - i).digitValue();
        if (mode != 0 && mode != 1) {
            return "";
        } else if (mode == 1) {
            decValue = decValue + pow(2, i);
        }
    }
    QString decStr = QString::number(decValue);
    return decStr;
}

quint16 BitItemHelper::calculate16Bits(QTreeWidgetItem *item)
{
    quint16 result = 0;
    for (int i = 0; i < item->childCount(); i++) {
        int start = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITSTART).toInt();
        int length = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt();
        uint16_t value = item->child(i)->text(GT::COL_FLASH_RAM_TREE_VALUE).toUShort();
        quint16 opera = getOperationValue(start, length);
        result = result + ((value << start) & opera);
        item->child(i)->setTextColor(GT::COL_FLASH_RAM_TREE_VALUE, Qt::black);
    }
    return result;
}

quint32 BitItemHelper::calculate32Bits(QTreeWidgetItem *item)
{
    quint32 result = 0;
    for (int i = 0; i < item->childCount(); i++) {
        int start = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITSTART).toInt();
        int length = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt();
        uint32_t value = item->child(i)->text(GT::COL_FLASH_RAM_TREE_VALUE).toULong();
        quint32 opera = getOperationValue(start, length);
        result = result + ((value << start) & opera);
        item->child(i)->setTextColor(GT::COL_FLASH_RAM_TREE_VALUE, Qt::black);
    }
    return result;
}

quint64 BitItemHelper::calculate64Bits(QTreeWidgetItem *item)
{
    quint64 result = 0;
    for (int i = 0; i < item->childCount(); i++) {
        int start = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITSTART).toInt();
        int length = item->child(i)->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt();
        uint64_t value = item->child(i)->text(GT::COL_FLASH_RAM_TREE_VALUE).toULongLong();
        quint64 opera = getOperationValue(start, length);
        result = result + ((value << start) & opera);
        item->child(i)->setTextColor(GT::COL_FLASH_RAM_TREE_VALUE, Qt::black);
    }
    return result;
}
