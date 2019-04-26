#include "homeitem15.h"
#include "sevdevice.h"

HomeItem15::HomeItem15(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 15;
    setObjectName(tr("15 Reserved"));
}

HomeItem15::~HomeItem15()
{

}

bool HomeItem15::execute()
{
    m_homeConfig = 0;
    return true;
}

