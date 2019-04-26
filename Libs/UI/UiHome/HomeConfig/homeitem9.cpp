#include "homeitem9.h"
#include "sevdevice.h"

HomeItem9::HomeItem9(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 9;
    setObjectName(tr("9 Homing on first index mark after engaging positive side of home switch (home switch at middle of travel); initial move positive"));
}

HomeItem9::~HomeItem9()
{

}

bool HomeItem9::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

