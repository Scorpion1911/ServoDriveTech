#include "encconfighechuanitem.h"

EncConfigHeChuanItem::EncConfigHeChuanItem(QObject *parent) : IEncConfigItem(parent)
{
    m_encConfigData = 0x0006;
    setObjectName(tr("EncHeChuan"));
}

EncConfigHeChuanItem::~EncConfigHeChuanItem()
{

}

