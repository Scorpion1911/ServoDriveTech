#include "encconfigsinz.h"
#include "sdtglobaldef.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>

EncConfigSinZ::EncConfigSinZ(QObject *parent) : IEncConfigItem(parent),
    m_sinPerR(NULL)
{
    m_encType = ENC_TYPE_SIN;
    m_encConfigData = 0x000f;
    m_comboType = 1;
    m_isLineNumEditable = false;
    m_adGain = 4;
    m_sinConf = 8;
    setObjectName(tr("SinZ"));
}

EncConfigSinZ::~EncConfigSinZ()
{
    GT::deletePtrObject(m_attributeUi);
}

void EncConfigSinZ::createAttributeUi()
{
    m_attributeUi = new QWidget;
    QHBoxLayout *hBox = new QHBoxLayout(m_attributeUi);
    QLabel* sinLabel = new QLabel(tr("sinPerR"));
    m_sinPerR = new QSpinBox;
    connect(m_sinPerR, SIGNAL(valueChanged(int)), this, SLOT(onSinNumChanged(int)));
    //m_sinPerR->setValue(1);
    m_sinPerR->setMinimum(1);
    m_sinPerR->setMaximum(999999999);
    m_sinPerR->setButtonSymbols(QAbstractSpinBox::NoButtons);

    hBox->addWidget(sinLabel, 0);
    hBox->addWidget(m_sinPerR, 1);
    m_attributeUi->setLayout(hBox);
}

void EncConfigSinZ::updateAttributeUi()
{
    int num = m_lineNumber / pow(2, 12);
    m_sinPerR->setValue(num);
}

bool EncConfigSinZ::execute()
{
    m_encConfigData=0x000f;
    m_comboType = 1;
    m_adGain = 4;
    m_sinConf = 8;
    return true;
}

void EncConfigSinZ::onSinNumChanged(int num)
{
    int finalNum = num * pow(2, 12);
    emit lineNumChanged(finalNum);
}

