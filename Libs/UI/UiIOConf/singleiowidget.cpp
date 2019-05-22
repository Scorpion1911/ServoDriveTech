#include "singleiowidget.h"
#include "ui_singleiowidget.h"
#include "gtutils.h"

#include <QPalette>
#include <QDebug>

SingleIOWidget::SingleIOWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleIOWidget)
{
    ui->setupUi(this);
    QString onPath = GTUtils::iconPath() + "led_on.png";
    QString offPath = GTUtils::iconPath() + "led_off.png";
    QString str = "  QRadioButton::indicator::unchecked {\
        image: url(" + offPath + ");\
    }\
    QRadioButton::indicator::checked {\
        image: url("+ onPath + ");\
    }";
    ui->btn_status->setStyleSheet(str);
    connect(ui->checkBox_reverse, SIGNAL(toggled(bool)), this, SLOT(onReverseBoxChanged(bool)));
    connect(ui->btn_status, SIGNAL(clicked(bool)), this, SLOT(onEnStatusChanged(bool)));
    m_preFuncIndex = 0;
    m_curFunIndex = 0;
    ui->label->setVisible(false);
    ui->comboBox_controlSrc->setVisible(false);
}

SingleIOWidget::~SingleIOWidget()
{
    delete ui;
}

void SingleIOWidget::setIOStatus(bool en)
{
    ui->btn_status->setChecked(en);
}

void SingleIOWidget::setIOName(const QString &ioName)
{
    ui->btn_status->setText(ioName);
}

void SingleIOWidget::setIOFuncList(const QStringList &ioFuncList)
{
    ui->comboBox_funcList->addItems(ioFuncList);
    connect(ui->comboBox_funcList, SIGNAL(currentIndexChanged(int)), this, SLOT(onBoxIndexChanged(int)));
}

void SingleIOWidget::setPinName(const QString &pinName)
{
    ui->label_pin->setText(pinName);
}

void SingleIOWidget::setIONote(const QString &ioNote)
{
    ui->label_note->setText(ioNote);
}

void SingleIOWidget::setReverseStatus(bool en)
{
    ui->checkBox_reverse->setChecked(en);
}

void SingleIOWidget::setFuncListEnable(bool en)
{
    ui->comboBox_funcList->setEnabled(en);
}

void SingleIOWidget::setFuncIndex(int index)
{
    if (index >= ui->comboBox_funcList->count()) {
        index = 0;
    }
    ui->comboBox_funcList->setCurrentIndex(index);
    ui->comboBox_funcList->setStyleSheet("");
}

void SingleIOWidget::setBoxIndex(int index)
{
    m_index = index;
}

void SingleIOWidget::setCheckable(bool en)
{
    ui->btn_status->setEnabled(en);
}

void SingleIOWidget::setControlSrcList(const QStringList &srcList)
{
    ui->comboBox_controlSrc->addItems(srcList);
    connect(ui->comboBox_controlSrc, SIGNAL(currentIndexChanged(int)), this, SLOT(onControlSrcChanged(int)));
}

void SingleIOWidget::setControlSrc(int index)
{
    if (index >= ui->comboBox_controlSrc->count()) {
        index = 0;
    }
    ui->comboBox_controlSrc->setCurrentIndex(index);
    ui->comboBox_controlSrc->setStyleSheet("");
}

bool SingleIOWidget::getReverseStatus()
{
    ui->checkBox_reverse->setStyleSheet("");
    return ui->checkBox_reverse->isChecked();
}

bool SingleIOWidget::getIOStatus()
{
    return ui->btn_status->isChecked();
}

int SingleIOWidget::getFuncIndex()
{
    return ui->comboBox_funcList->currentIndex();
}

int SingleIOWidget::getBoxIndex()
{
    return m_index;
}

int SingleIOWidget::getControlSrc()
{
    return ui->comboBox_controlSrc->currentIndex();
}

void SingleIOWidget::onBoxIndexChanged(int index)
{
    ui->comboBox_funcList->setStyleSheet("QComboBox { background-color: yellow; }");
    m_preFuncIndex = m_curFunIndex;
    m_curFunIndex = index;
    emit funcIndexChanged(index, m_preFuncIndex, m_index);
}

void SingleIOWidget::onControlSrcChanged(int index)
{
    ui->comboBox_controlSrc->setStyleSheet("QComboBox { background-color: yellow; }");
    emit controlSrcChanged(index);
}

void SingleIOWidget::onReverseBoxChanged(bool checked)
{
    ui->checkBox_reverse->setStyleSheet("QCheckBox{color:red}");
    emit reverseStatusChanged(checked, m_index);
}

void SingleIOWidget::onEnStatusChanged(bool en)
{
    emit enStatusChanged(m_index, en);
}
