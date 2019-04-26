#include "homeitem35.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QMessageBox>

HomeItem35::HomeItem35(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 35;
    setObjectName(tr("35 Declare present position as home"));
}

HomeItem35::~HomeItem35()
{

}

bool HomeItem35::execute()
{
    m_homeConfig = 0;
    return true;
}

