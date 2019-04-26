#include "homeitem33.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QMessageBox>

HomeItem33::HomeItem33(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 33;
    setObjectName(tr("33 Homing on index mark, moving negative"));
}

HomeItem33::~HomeItem33()
{

}

bool HomeItem33::execute()
{
    m_homeConfig = 16 + (m_axisInx << 8) + (14 << 12);
    return true;
}

