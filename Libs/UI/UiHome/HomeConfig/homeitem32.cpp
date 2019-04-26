#include "homeitem32.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QMessageBox>

HomeItem32::HomeItem32(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 32;
    setObjectName(tr("32 Reserved"));
}

HomeItem32::~HomeItem32()
{

}

bool HomeItem32::execute()
{
    m_homeConfig = 0;
    return true;
}

