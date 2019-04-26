#include "homeitem0.h"
#include "sevdevice.h"

HomeItem0::HomeItem0(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 0;
    setObjectName(tr("0 SD15 Homing"));
}

HomeItem0::~HomeItem0()
{
    
}

bool HomeItem0::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

