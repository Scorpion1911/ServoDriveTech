#include "encconfigabz.h"

EncConfigABZ::EncConfigABZ(QObject *parent) : IEncConfigItem(parent)
{
    m_encType = ENC_TYPE_ABZ;
    m_encConfigData = 0x000f;
    m_comboType = 5;
    setObjectName(tr("EncABZ"));
}

EncConfigABZ::~EncConfigABZ()
{

}

