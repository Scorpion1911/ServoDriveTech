#include "downloaddialog.h"
#include "ui_downloaddialog.h"
#include "sevdevice.h"

#include <QFileInfo>
#include <QFileDialog>

DownloadDialog::DownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownloadDialog)
{
    ui->setupUi(this);
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::uiInit(const QList<SevDevice *> &devList, const QString &downloadPath, QString &filePath, QList<int> &indexList)
{
    m_downloadPath = downloadPath;
    m_filePath = &filePath;
    m_indexList = &indexList;
    m_devNum = devList.count();
    QVBoxLayout *vBox = new QVBoxLayout;
    for (int i = 0; i < m_devNum; i++) {
        bool hasNickName = m_devNum > 1;
        QString prefix;
        prefix = hasNickName?tr("[%1] ").arg(devList.at(i)->aliasName()):"";
        QCheckBox *box = new QCheckBox(prefix + devList.at(i)->modelName());
        m_boxList.append(box);
        vBox->addWidget(box);
        connect(box, SIGNAL(clicked(bool)), this, SLOT(onSingleBoxClicked(bool)));
    }
    vBox->setSpacing(2);
    ui->scrollAreaWidgetContents_dev->setLayout(vBox);
    ui->lineEdit_fileDownload->setReadOnly(true);
    connect(ui->toolBtn_download, SIGNAL(clicked()), this, SLOT(onToolButtonClicked()));
    connect(ui->btn_okDownload, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(ui->checkBox_all, SIGNAL(clicked(bool)), this, SLOT(onAllBoxClicked(bool)));
}

void DownloadDialog::onToolButtonClicked()
{
    *m_filePath = QFileDialog::getOpenFileName(this, tr("Open XML File"), m_downloadPath, tr("XML Files(*.xml)"));
    QFileInfo fileInfo;
    fileInfo.setFile(*m_filePath);
    ui->lineEdit_fileDownload->setText(fileInfo.fileName());
}

void DownloadDialog::onOkButtonClicked()
{
    this->close();
}

void DownloadDialog::onAllBoxClicked(bool checked)
{
    m_indexList->clear();
    for (int i = 0; i < m_devNum; i++) {
        m_boxList.at(i)->setChecked(checked);
        m_indexList->append(i);
    }
}

void DownloadDialog::onSingleBoxClicked(bool checked)
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
