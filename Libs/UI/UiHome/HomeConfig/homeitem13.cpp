#include "homeitem13.h"
#include "sevdevice.h"

HomeItem13::HomeItem13(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 13;
    setObjectName(tr("13 Homing on first index mark after engaging negative side of home switch (home switch at middle of travel); initial move negative"));
}

HomeItem13::~HomeItem13()
{

}

bool HomeItem13::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

