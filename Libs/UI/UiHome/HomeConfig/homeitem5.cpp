#include "homeitem5.h"
#include "sevdevice.h"

HomeItem5::HomeItem5(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 5;
    setObjectName(tr("5 Homing on first index mark after disengaging from home switch (home switch at negative travel)"));
}

HomeItem5::~HomeItem5()
{

}

bool HomeItem5::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}
