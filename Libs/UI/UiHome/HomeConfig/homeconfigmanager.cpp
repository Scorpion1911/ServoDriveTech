#include "homeconfigmanager.h"
#include "ihomeconfigitem.h"
#include "sdtglobaldef.h"

HomeConfigManager::HomeConfigManager(QObject *parent) : QObject(parent)
{

}

HomeConfigManager::~HomeConfigManager()
{
    GT::deepClearList(m_homeItemList);
}

void HomeConfigManager::addHomeItem(IHomeConfigItem *item)
{
    m_homeItemList.append(item);
}

IHomeConfigItem *HomeConfigManager::homeItem(quint8 index)
{
    return m_homeItemList.at(index);
}

QStringList HomeConfigManager::itemNames()
{
    QStringList result;
    for (int i = 0; i < m_homeItemList.count(); i++) {
        IHomeConfigItem *item = m_homeItemList.at(i);
        result.append(item->objectName());
    }
    return result;
}

void HomeConfigManager::clearAllHomeItems()
{
    GT::deepClearList(m_homeItemList);
}

