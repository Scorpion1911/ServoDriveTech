#include "advuserfirmwaresegmentflash.h"
#include "ui_advuserfirmwaresegmentflash.h"

#include "iadvuser_p.h"
#include <QString>
#include <QStringList>
#include <QProgressBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>

#include <QList>
#include "sevdevice.h"
#include "gtutils.h"
#include <QTreeWidget>
#include <QtTreeManager/qttreemanager.h>
#include <QTreeWidgetItem>



class AdvUserFirmwareSegmentFlashPrivate : public IAdvUserPrivate
{
   Q_DECLARE_PUBLIC(AdvUserFirmwareSegmentFlash)

public:
    AdvUserFirmwareSegmentFlashPrivate();
    ~AdvUserFirmwareSegmentFlashPrivate();
    QString m_hexFilePath;
    QString m_rpdFilePath;
    QStringList m_xmlFilePaths;
    QList<SevDevice *> m_devList;
    SevDevice* m_crtDev;

};

AdvUserFirmwareSegmentFlashPrivate::AdvUserFirmwareSegmentFlashPrivate()
{

}
AdvUserFirmwareSegmentFlashPrivate::~AdvUserFirmwareSegmentFlashPrivate()
{

}

AdvUserFirmwareSegmentFlash::AdvUserFirmwareSegmentFlash(const QString &advUserName, QWidget *parent) :
    IAdvUser(advUserName, *new AdvUserFirmwareSegmentFlashPrivate, parent),
    ui(new Ui::AdvUserFirmwareSegmentFlash)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    uiInit();

    QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onActionComboBoxIndexChange(int)));

    QObject::connect(ui->toolButtonDSP,SIGNAL(clicked()),this,SLOT(onActionDSPToolBtnClicked()));
    QObject::connect(ui->toolButtonFPGA,SIGNAL(clicked()),this,SLOT(onActionFPGAToolBtnClicked()));
    QObject::connect(ui->toolButtonFLASH,SIGNAL(clicked()),this,SLOT(onActionFLASHToolBtnClicked()));

    QObject::connect(ui->pushButtonDSP,SIGNAL(clicked()),this,SLOT(onActionDSPFlashBtnClicked()));
    QObject::connect(ui->pushButtonFPGA,SIGNAL(clicked()),this,SLOT(onActionFPGAFlashBtnClicked()));
    QObject::connect(ui->pushButtonFLASH,SIGNAL(clicked()),this,SLOT(onActionFLASHFlashBtnClicked()));
}

AdvUserFirmwareSegmentFlash::~AdvUserFirmwareSegmentFlash()
{
    delete ui;
}

QString AdvUserFirmwareSegmentFlash::nickName()
{
    return tr("FirmFlash");
}

void AdvUserFirmwareSegmentFlash::uiInit()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    for(int i = 0; i < d->m_devList.length(); i++){
        ui->comboBox->addItem(d->m_devList.at(i)->modelName());
    }
    ui->comboBox->setCurrentIndex(0);
//    d->m_crtDev = d->m_devList.at(0);
    ui->plainTextEdit->clear();
    ui->pushButtonDSP->setEnabled(false);
    ui->pushButtonFPGA->setEnabled(false);
    ui->pushButtonFLASH->setEnabled(false);

}

void AdvUserFirmwareSegmentFlash::setSevList(const QList<SevDevice *> &list)
{
    Q_D(AdvUserFirmwareSegmentFlash);
    d->m_devList = list;
}

bool AdvUserFirmwareSegmentFlash::advUserActive()
{
    return true;
}

bool AdvUserFirmwareSegmentFlash::readAdv()
{
    return true;
}

bool AdvUserFirmwareSegmentFlash::writeAdv()
{
    return true;
}

void AdvUserFirmwareSegmentFlash::respondErrorExecute()
{
    return;
}

void AdvUserFirmwareSegmentFlash::updateProgressBar(void *arg, qint16 *value)
{
    QProgressBar *progressBar = static_cast<QProgressBar *>(arg);
    progressBar->setValue(*value);
    qApp->processEvents();
}

void AdvUserFirmwareSegmentFlash::onActionComboBoxIndexChange(int index)
{
    Q_D(AdvUserFirmwareSegmentFlash);
    d->m_crtDev = d->m_devList.at(index);
}

void AdvUserFirmwareSegmentFlash::onActionDSPToolBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    QString hexFilePath = QFileDialog::getOpenFileName(this, tr("Please select one HEX type file!"), "C:/Users/googol/Desktop", tr("HEX File(*.hex)"));
    bool ok = true;
    if(hexFilePath.compare("")==0){
        QMessageBox::warning(this, tr("Warning!"), tr("NO file selected!"), QMessageBox::Ok);
        ok = false;
    }
    if(ok){
        d->m_hexFilePath = hexFilePath;
        ui->plainTextEdit->appendPlainText(tr("Selected HEX type file:\n\t%1").arg(d->m_hexFilePath));
        QFileInfo fileinfo;
        fileinfo.setFile(d->m_hexFilePath);
        ui->lineEditDSP->setText(fileinfo.fileName());
        ui->pushButtonDSP->setEnabled(true);
    }else{
        ui->pushButtonDSP->setEnabled(false);
    }
}

void AdvUserFirmwareSegmentFlash::onActionFPGAToolBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    QString rpdFilePath = QFileDialog::getOpenFileName(this, tr("Please select one RPD type file!"), "C:/Users/googol/Desktop", tr("RPD File(*.rpd)"));
    bool ok = true;
    if(rpdFilePath.compare("")==0){
        QMessageBox::warning(this, tr("Warning!"), tr("NO file selected!"), QMessageBox::Ok);
        ok = false;
    }
    if(ok){
        d->m_rpdFilePath = rpdFilePath;
        ui->plainTextEdit->appendPlainText(tr("Selected RPD type file:\n\t%1").arg(d->m_rpdFilePath));
        QFileInfo fileinfo;
        fileinfo.setFile(d->m_rpdFilePath);
        ui->lineEditFPGA->setText(fileinfo.fileName());
        ui->pushButtonFPGA->setEnabled(true);
    }else{
        ui->pushButtonFPGA->setEnabled(false);
    }
}

void AdvUserFirmwareSegmentFlash::onActionFLASHToolBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    QStringList xmlFilePaths = QFileDialog::getOpenFileNames(this, tr("Please select three XML type files!"), "C:/Users/googol/Desktop", tr("XML File(*.xml)"));
    QFileInfo fileinfo;
    bool ok = true;
    if(xmlFilePaths.count() != 3)
    {
        QMessageBox::warning(this, tr("Warning!"), tr("The number of documents selected is not three;\nPlease select again!"), QMessageBox::Ok);
        ok = false;
    }
    if(ok){
        char *fileFlag = new char[3];
        memset(fileFlag,0,3);
        QString str = "The following files are missing from the selected file :";
        for(int i = 0; i < 3; i++){
            fileinfo.setFile(xmlFilePaths.at(i));
            if(fileinfo.fileName().compare("FILENAME_XML_FLASHPRM.xml") == 0)
                fileFlag[0] += 1;
            if(fileinfo.fileName().compare("FILENAME_XML_RAMPRM0.xml") == 0)
                fileFlag[1] += 1;
            if(fileinfo.fileName().compare("FILENAME_XML_RAMPRM1.xml") == 0)
                fileFlag[2] += 1;
        }
        if(fileFlag[0] == 0||fileFlag[1] == 0||fileFlag[2] == 0){
            if(fileFlag[0] == 0)
                str += "\n\tFILENAME_XML_FLASHPRM.xml;";
            if(fileFlag[1] == 0)
                str += "\n\tFILENAME_XML_RAMPRM0.xml;";
            if(fileFlag[2] == 0)
                str += "\n\tFILENAME_XML_RAMPRM1.xml;";
            QMessageBox::warning(this, tr("Warning!"), str+"\nPlease select again!", QMessageBox::Ok);
            ok = false;
        }
        delete fileFlag;
    }
    if(ok){
        d->m_xmlFilePaths = xmlFilePaths;
        for(int i=0; i < 3; i++){
            fileinfo.setFile(d->m_xmlFilePaths.at(i));
            if(i==0){
                ui->plainTextEdit->appendPlainText(tr("Selected XML type files:\n\t(%1)%2").arg(i+1).arg(d->m_xmlFilePaths.at(i)));
                ui->lineEditFLASH->setText(fileinfo.fileName() + ";");
            }else{
                ui->plainTextEdit->appendPlainText(tr("\t(%1)%2").arg(i+1).arg(d->m_xmlFilePaths.at(i)));
                ui->lineEditFLASH->insert(fileinfo.fileName() + ";");
            }
        }
        ui->pushButtonFLASH->setEnabled(true);
    }else{
        ui->pushButtonFLASH->setEnabled(false);
    }
}

void AdvUserFirmwareSegmentFlash::onActionDSPFlashBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    ui->progressBar->setVisible(true);
    ui->plainTextEdit->appendPlainText(tr("Start download dsp!"));
    bool ok = true;
    if(!d->m_crtDev->isConnecting()){
        QMessageBox::warning(this, tr("Warning!"), tr("Device:%1 does not connect!").arg(d->m_crtDev->modelName()), QMessageBox::Ok);
        ok = false;
    }
    int dspNum = 0;
    if(ok){
        QString filePath = GTUtils::sysPath() + d->m_crtDev->typeName() + "/" + d->m_crtDev->modelName() + "/"\
                            + d->m_crtDev->versionName() + "/" + "PrmFirmwareUpdate.xml";
        QTreeWidget *tree = QtTreeManager::createTreeWidgetFromXmlFile(filePath);
        QTreeWidgetItem *dspNameItem = GTUtils::findItem("dsp", tree, 0);
        if(dspNameItem != NULL){
            dspNum = dspNameItem->text(1).toInt();
        }else{
            dspNum = (d->m_crtDev->axisNum() + 1) / 2;
        }
        delete tree;
        for(int i = 0; i < dspNum; i++){
            qint16 ret = d->m_crtDev->socketCom()->downLoadDSPFLASH(i, d->m_hexFilePath.toStdWString(), updateProgressBar, (void *)ui->progressBar);
            if(ret != 0){
                ui->plainTextEdit->appendPlainText(tr("Download dsp:%1 fail!").arg(i+1));
                ok = false;
            }
        }
    }
    if(ok){
        ui->plainTextEdit->appendPlainText(tr("Download dsp successful!"));
    }
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    qApp->processEvents();
}

void AdvUserFirmwareSegmentFlash::onActionFPGAFlashBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    ui->progressBar->setVisible(true);
    ui->plainTextEdit->appendPlainText(tr("Start download fpga!"));
    bool ok = true;
    if(!d->m_crtDev->isConnecting()){
        QMessageBox::warning(this, tr("Warning!"), tr("Device:%1 does not connect!").arg(d->m_crtDev->modelName()), QMessageBox::Ok);
        ok = false;
    }
    int fpgaNum = 0;
    int fpgaAxisNum = 0;
    if(ok){
        QString filePath = GTUtils::sysPath() + d->m_crtDev->typeName() + "/" + d->m_crtDev->modelName() + "/"\
                            + d->m_crtDev->versionName() + "/" + "PrmFirmwareUpdate.xml";
        QTreeWidget *tree = QtTreeManager::createTreeWidgetFromXmlFile(filePath);
        QTreeWidgetItem *fpgaNameItem = GTUtils::findItem("fpga", tree, 0);
        if(fpgaNameItem != NULL){
            fpgaNum = fpgaNameItem->text(1).toInt();
            fpgaAxisNum = fpgaNameItem->child(0)->text(1).toInt();
        }else{
            fpgaNum = 1;
            fpgaNum = d->m_crtDev->axisNum();
        }
        delete tree;
        for(int i = 0; i < fpgaNum; i++){
            qint16 ret = d->m_crtDev->socketCom()->downLoadFPGAFLASH(i * fpgaAxisNum, d->m_hexFilePath.toStdWString(), updateProgressBar, (void *)ui->progressBar);
            if(ret != 0){
                ui->plainTextEdit->appendPlainText(tr("Download fpga fail!"));
                ok = false;
            }
        }
    }
    if(ok){
       ui->plainTextEdit->appendPlainText(tr("Download fpga successful!"));
    }
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    qApp->processEvents();
}

void AdvUserFirmwareSegmentFlash::onActionFLASHFlashBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    ui->progressBar->setVisible(true);
    ui->plainTextEdit->appendPlainText(tr("Start download flash!"));
    bool ok = true;
    if(!d->m_crtDev->isConnecting()){
        QMessageBox::warning(this, tr("Warning!"), tr("Device:%1 does not connect!").arg(d->m_crtDev->modelName()), QMessageBox::Ok);
        ok = false;
    }
    quint8 axis = 0;
    short value = ui->progressBar->value();
    QList<int> fileTypeList;
    fileTypeList<<0<<0<<0;
    ok = d->m_crtDev->writeXml(axis, d->m_xmlFilePaths, fileTypeList, d->m_xmlFilePaths.length(), updateProgressBar, (void *)ui->progressBar, value);
    if(ok){
        ui->plainTextEdit->appendPlainText(tr("Download flash successful!"));
    }else{
        ui->plainTextEdit->appendPlainText(tr("Download flash fail!"));
    }
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    qApp->processEvents();
}


