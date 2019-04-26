#include "homeitem11.h"
#include "sevdevice.h"

HomeItem11::HomeItem11(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 11;
    setObjectName(tr("11 Homing on first index mark after disengaging from positive side of home switch (home switch at middle of travel); initial move negative"));
}

HomeItem11::~HomeItem11()
{

}

bool HomeItem11::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

