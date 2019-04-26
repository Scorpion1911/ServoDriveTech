#include "homeitem16.h"
#include "sevdevice.h"

HomeItem16::HomeItem16(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 16;
    setObjectName(tr("16 Reserved"));
}

HomeItem16::~HomeItem16()
{

}

bool HomeItem16::execute()
{
    m_homeConfig = 0;
    return true;
}

