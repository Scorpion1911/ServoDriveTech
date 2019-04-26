#include "homeitem31.h"
#include "sevdevice.h"
#include "bititemhelper.h"

#include <QMessageBox>

HomeItem31::HomeItem31(int axisInx, SevDevice *dev, QObject *parent) : IHomeConfigItem(axisInx, dev, parent)
{
    m_homeType = 31;
    setObjectName(tr("31 Reserved"));
}

HomeItem31::~HomeItem31()
{

}

bool HomeItem31::execute()
{
    m_homeConfig = 0;
    return true;
}

