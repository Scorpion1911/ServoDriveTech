#include "ihomeconfigitem.h"

IHomeConfigItem::IHomeConfigItem(QObject *parent) : QObject(parent),
    m_homeConfig(0),
    m_homeType(0)
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

