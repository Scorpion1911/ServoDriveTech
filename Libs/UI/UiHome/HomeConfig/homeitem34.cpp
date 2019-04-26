#include "homeitem34.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QMessageBox>

HomeItem34::HomeItem34(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 34;
    setObjectName(tr("34 Homing on index mark, moving positive"));
}

HomeItem34::~HomeItem34()
{

}

bool HomeItem34::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

