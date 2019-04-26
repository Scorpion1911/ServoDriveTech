#include "homeitem3.h"
#include "sevdevice.h"

HomeItem3::HomeItem3(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 3;
    setObjectName(tr("3 Homing on first index mark after disengaging from home switch (home switch at positive travel)"));
}

HomeItem3::~HomeItem3()
{

}

bool HomeItem3::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

