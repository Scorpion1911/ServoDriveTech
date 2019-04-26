#include "homeitem8.h"
#include "sevdevice.h"

HomeItem8::HomeItem8(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 8;
    setObjectName(tr("8 Homing on first index mark after engaging negative side of home switch (home switch at middle of travel); initial move positive"));
}

HomeItem8::~HomeItem8()
{

}

bool HomeItem8::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}


