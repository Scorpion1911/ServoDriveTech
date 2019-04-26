#include "homeitem6.h"
#include "sevdevice.h"

HomeItem6::HomeItem6(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 6;
    setObjectName(tr("6 Homing on first index mark after engaging home switch (home switch at negative travel)"));
}

HomeItem6::~HomeItem6()
{

}

bool HomeItem6::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

