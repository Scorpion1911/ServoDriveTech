#include "homeitem4.h"
#include "sevdevice.h"

HomeItem4::HomeItem4(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 4;
    setObjectName(tr("4 Homing on first index mark after engaging home switch (home switch at positive travel)"));
}

HomeItem4::~HomeItem4()
{

}

bool HomeItem4::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

