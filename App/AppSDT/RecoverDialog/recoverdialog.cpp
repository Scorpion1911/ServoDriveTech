#include "recoverdialog.h"
#include "ui_recoverdialog.h"

#include <QCheckBox>

#include "sevdevice.h"

RecoverDialog::RecoverDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecoverDialog)
{
    ui->setupUi(this);
}

RecoverDialog::~RecoverDialog()
{
    delete ui;
}

void RecoverDialog::uiInit(const QList<SevDevice *> &devList, QList<int> &indexList)
{
    m_indexList = &indexList;
    m_devNum = devList.count();
    QVBoxLayout *vBox = new QVBoxLayout;
    for (int i = 0; i < m_devNum; i++) {
        bool hasNickName = true;
        QString prefix;
        prefix = hasNickName?tr("[%1] ").arg(devList.at(i)->aliasName()):"";
        QCheckBox *box = new QCheckBox(prefix + devList.at(i)->modelName());
        m_boxList.append(box);
        vBox->addWidget(box);
        connect(box, SIGNAL(clicked(bool)), this, SLOT(onSingleBoxClicked(bool)));
    }
    vBox->setSpacing(2);
    ui->scrollAreaWidgetContents->setLayout(vBox);
    connect(ui->btn_Ok, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(ui->checkBox_all, SIGNAL(clicked(bool)), this, SLOT(onAllBoxClicked(bool)));
}

void RecoverDialog::onSingleBoxClicked(bool checked)
{
    QObject *pObject = this->sender();
    QCheckBox *pBox = qobject_cast<QCheckBox *>(pObject);
    int index = m_boxList.indexOf(pBox);
    if (checked) {
        m_indexList->append(index);
    } else {
        m_indexList->removeOne(index);
    }
    if (m_indexList->count() == m_devNum) {
        ui->checkBox_all->setChecked(true);
    } else {
        ui->checkBox_all->setChecked(false);
    }
}

void RecoverDialog::onOkButtonClicked()
{
    this->close();
}

void RecoverDialog::onAllBoxClicked(bool checked)
{
    m_indexList->clear();
    for (int i = 0; i < m_devNum; i++) {
        m_boxList.at(i)->setChecked(checked);
        if (checked) {
            m_indexList->append(i);
        }
    }
}
