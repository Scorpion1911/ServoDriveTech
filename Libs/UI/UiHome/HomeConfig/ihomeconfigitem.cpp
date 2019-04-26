#include "ihomeconfigitem.h"
#include "sevdevice.h"

IHomeConfigItem::IHomeConfigItem(int axisInx, SevDevice *dev, QObject *parent) : QObject(parent),
    m_axisInx(axisInx),
    m_homeConfig(0),
    m_homeType(0),
    m_reverse(false),
    m_dev(dev)
{
    setObjectName(tr("IHomeConfig"));
}

IHomeConfigItem::~IHomeConfigItem()
{

}

bool IHomeConfigItem::execute()
{
    return true;
}

int IHomeConfigItem::getHomeType()
{
    return m_homeType;
}

void IHomeConfigItem::setHomeType(int type)
{
    m_homeType = type;
}

quint16 IHomeConfigItem::getHomeConfig()
{
    return m_homeConfig;
}

void IHomeConfigItem::setHomeConfig(quint16 config)
{
    m_homeConfig = config;
}

bool IHomeConfigItem::getReverse()
{
    return m_reverse;
}

void IHomeConfigItem::setReverse(bool en)
{
    m_reverse = en;
}

