#include "homeitem7.h"
#include "sevdevice.h"

HomeItem7::HomeItem7(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 7;
    setObjectName(tr("7 Homing on first index mark after disengaging from negative side of home switch (home switch at middle of travel); initial move positive"));
}

HomeItem7::~HomeItem7()
{

}

bool HomeItem7::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

