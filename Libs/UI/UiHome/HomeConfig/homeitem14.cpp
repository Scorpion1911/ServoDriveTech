#include "homeitem14.h"
#include "sevdevice.h"

HomeItem14::HomeItem14(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 14;
    setObjectName(tr("14 Homing on first index mark after disengaging from negative side of home switch (home switch at middle of travel); initial move negative"));
}

HomeItem14::~HomeItem14()
{

}

bool HomeItem14::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}
