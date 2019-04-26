#include "homeitem20.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QMessageBox>

HomeItem20::HomeItem20(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 20;
    setObjectName(tr("20 Homing on rising edge of home switch (home switch at positive travel)."));
}

HomeItem20::~HomeItem20()
{

}

bool HomeItem20::execute()
{
    quint16 trigSrc = 0;
    quint16 trigMode = 0;
    int negativeIO = 0;
    quint16 input = 0;
    m_dev->readFLASH16ByAddr(m_axisInx, HOME_SWITCH_ADDR, input);
    if (input != 0) {
        negativeIO = log2(input);
        trigSrc = 16 + negativeIO;
    } else {
        QMessageBox::warning(0, tr("Warning"), tr("Please set an input for home switch!"));
        return false;
    }
    quint16 reverse = 0;
    m_dev->readFLASH16ByAddr(m_axisInx, INPUT_REVERSE_ADDR, reverse);
    QString reverseStr = BitItemHelper::decToBin16(QString::number(reverse));
    int bitValue = reverseStr.at(negativeIO).digitValue();
    if (bitValue == 1) {
        trigMode = 10;
    } else {
        trigMode = 14;
    }
    m_homeConfig = trigSrc + (m_axisInx << 8) + (trigMode << 12);
    return true;
}

