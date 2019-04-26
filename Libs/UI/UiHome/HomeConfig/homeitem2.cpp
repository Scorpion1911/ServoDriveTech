#include "homeitem2.h"
#include "sevdevice.h"

HomeItem2::HomeItem2(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 2;
    setObjectName(tr("2 Homing on first index mark after disengaging from positive limit"));
}

HomeItem2::~HomeItem2()
{

}

bool HomeItem2::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}
