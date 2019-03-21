#include "encconfigrotation.h"
#include "sdtglobaldef.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>

EncConfigRotation::EncConfigRotation(QObject *parent) : IEncConfigItem(parent)
{
    m_encType = ENC_TYPE_ROTA;
    m_encConfigData = 0x0000;
    m_comboType = 4;
    m_adGain = 1;
    m_sinConf = 4;
    m_isLineNumEditable = false;
    setObjectName(tr("EncRotation"));
}

EncConfigRotation::~EncConfigRotation()
{
    GT::deletePtrObject(m_attributeUi);
}

void EncConfigRotation::createAttributeUi()
{
    m_attributeUi = new QWidget;
    QHBoxLayout *hBox = new QHBoxLayout(m_attributeUi);
    QLabel* sinLabel = new QLabel(tr("polesNum"));
    m_polesNum = new QSpinBox;
    connect(m_polesNum, SIGNAL(valueChanged(int)), this, SLOT(onPolesNumChanged(int)));
    //m_sinPerR->setValue(1);
    m_polesNum->setMinimum(1);
    m_polesNum->setMaximum(256);
    m_polesNum->setButtonSymbols(QAbstractSpinBox::NoButtons);

    hBox->addWidget(sinLabel, 0);
    hBox->addWidget(m_polesNum, 1);
    m_attributeUi->setLayout(hBox);
}

void EncConfigRotation::updateAttributeUi()
{
    int num = m_lineNumber / pow(2, 16);
    m_polesNum->setValue(num);
}

bool EncConfigRotation::execute()
{
    m_comboType = 4;
    m_adGain = 1;
    m_sinConf = 4;
    if (m_polesNum->value() == 1) {
        m_encConfigData = 0x0000;
    } else {
        m_encConfigData = 0x000f;
    }
    return true;
}

void EncConfigRotation::onPolesNumChanged(int num)
{
    int finalNum = num * pow(2, 16);
    emit lineNumChanged(finalNum);
}

