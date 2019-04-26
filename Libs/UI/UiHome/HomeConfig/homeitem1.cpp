#include "homeitem1.h"
#include "sevdevice.h"

HomeItem1::HomeItem1(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 1;
    setObjectName(tr("1 Homing on first index mark after disengaging from negative limit"));
}

HomeItem1::~HomeItem1()
{

}

bool HomeItem1::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}
