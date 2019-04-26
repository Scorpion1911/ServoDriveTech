#include "homeitem12.h"
#include "sevdevice.h"

HomeItem12::HomeItem12(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 12;
    setObjectName(tr("12 Homing on first index mark after engaging positive side of home switch (home switch at middle of travel); initial move negative."));
}

HomeItem12::~HomeItem12()
{

}

bool HomeItem12::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

