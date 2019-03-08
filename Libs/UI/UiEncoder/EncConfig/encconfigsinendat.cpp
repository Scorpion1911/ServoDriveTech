#include "encconfigsinendat.h"
#include "sdtglobaldef.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>

EncConfigSinEndat::EncConfigSinEndat(QObject *parent) : IEncConfigItem(parent)
{
    m_encType = ENC_TYPE_SIN;
    m_encConfigData = 0x0002;
    m_comboType = 2;
    m_isLineNumEditable = false;
    m_adGain = 4;
    m_sinConf = 8;
    m_needTimer = true;
    setObjectName(tr("EncSinEndat"));
}

EncConfigSinEndat::~EncConfigSinEndat()
{
    GT::deletePtrObject(m_attributeUi);
}

void EncConfigSinEndat::createAttributeUi()
{
    m_attributeUi = new QWidget;
    QVBoxLayout *vBox = new QVBoxLayout(m_attributeUi);
    QHBoxLayout *hBoxUp = new QHBoxLayout;
    QHBoxLayout *hBoxMiddle = new QHBoxLayout;
    QLabel* sinLabel = new QLabel(tr("sinPerR"));
    QLabel* absLabel = new QLabel(tr("Abs LineNum"));
    m_sinPerR = new QSpinBox;
    m_absLineNumBox = new QSpinBox;
    m_reverseBox = new QCheckBox(tr("Reverse"));
    connect(m_sinPerR, SIGNAL(valueChanged(int)), this, SLOT(onSinNumChanged(int)));
    //m_sinPerR->setValue(1);
    m_sinPerR->setMinimum(1);
    m_sinPerR->setMaximum(999999999);
    m_sinPerR->setButtonSymbols(QAbstractSpinBox::NoButtons);

    //m_absLineNumBox->setValue(1);
    m_absLineNumBox->setMinimum(1);
    m_absLineNumBox->setMaximum(999999999);
    m_absLineNumBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    QLabel* absPosLabel = new QLabel(tr("Abs Position"));
    m_absPos = new QSpinBox;
    m_absPos->setMinimum(1);
    m_absPos->setMaximum(999999999);
    m_absPos->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_absPos->setReadOnly(true);
    QHBoxLayout *hBoxDown = new QHBoxLayout;

    hBoxUp->addWidget(sinLabel, 0);
    hBoxUp->addWidget(m_sinPerR, 1);
    hBoxMiddle->addWidget(absLabel, 0);
    hBoxMiddle->addWidget(m_absLineNumBox, 1);
    hBoxDown->addWidget(absPosLabel, 0);
    hBoxDown->addWidget(m_absPos, 1);
    vBox->addLayout(hBoxUp);
    vBox->addLayout(hBoxMiddle);
    vBox->addLayout(hBoxDown);
    vBox->addWidget(m_reverseBox);
    m_attributeUi->setLayout(vBox);
}

void EncConfigSinEndat::updateAttributeUi()
{
    int num = m_lineNumber / pow(2, 12);
    m_sinPerR->setValue(num);
    m_absLineNumBox->setValue(m_absLineNum);
    int reverse = m_sinConf >> 15;
    if (reverse == 1) {
        m_reverseBox->setChecked(true);
    } else {
        m_reverseBox->setChecked(false);
    }
}

bool EncConfigSinEndat::execute()
{
    m_encConfigData=0x0002;
    m_comboType = 2;
    m_adGain = 4;
    m_absLineNum = m_absLineNumBox->value();
    if (m_reverseBox->isChecked()) {
        m_sinConf = 0x8008;
    } else {
        m_sinConf = 8;
    }
    return true;
}

void EncConfigSinEndat::setAbsPos(quint32 value)
{
    m_absPos->setValue(value);
}

void EncConfigSinEndat::onSinNumChanged(int num)
{
    int finalNum = num * pow(2, 12);
    emit lineNumChanged(finalNum);
}

