#include "homeitem10.h"
#include "sevdevice.h"

HomeItem10::HomeItem10(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 10;
    setObjectName(tr("10 Homing on first index mark after disengaging from positive side of home switch (home switch at middle of travel); initial move positive"));
}

HomeItem10::~HomeItem10()
{

}

bool HomeItem10::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}
