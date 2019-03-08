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
#include <QDir>




class AdvUserFirmwareSegmentFlashPrivate : public IAdvUserPrivate
{
   Q_DECLARE_PUBLIC(AdvUserFirmwareSegmentFlash)

public:
    AdvUserFirmwareSegmentFlashPrivate();
    ~AdvUserFirmwareSegmentFlashPrivate();
    QString m_openFilePath;
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
    //this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    uiInit();

    //QObject::connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onActionComboBoxIndexChange(int)));

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
    readAdv();
    d->m_crtDev = 0;
    disconnect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onActionComboBoxIndexChange(int)));
    ui->comboBox->clear();
    if(d->m_devList.count() != 0){
        d->m_crtDev = d->m_devList.at(0);
        for(int i = 0; i < d->m_devList.length(); i++){
            bool hasNickName = d->m_devList.count() > 1;
            QString prefix;
            prefix = hasNickName?tr("[%1] ").arg(d->m_devList.at(i)->aliasName()):"";
            ui->comboBox->addItem(prefix + d->m_devList.at(i)->modelName());
        }
    }
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(onActionComboBoxIndexChange(int)));
    ui->comboBox->setCurrentIndex(0);
    ui->lineEditDSP->clear();
    ui->lineEditFPGA->clear();
    ui->lineEditFLASH->clear();
    ui->plainTextEdit->clear();
    ui->pushButtonDSP->setEnabled(false);
    ui->pushButtonFPGA->setEnabled(false);
    ui->pushButtonFLASH->setEnabled(false);
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
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
    Q_D(AdvUserFirmwareSegmentFlash);
    d->m_openFilePath = dataFromFile("advFirmware", "openPath", ".").toString();
    QDir dir(d->m_openFilePath);
    if(!dir.exists()){
        d->m_openFilePath = ".";
    }
    return true;
}

bool AdvUserFirmwareSegmentFlash::writeAdv()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    saveDataToFile("advFirmware", "openPath", d->m_openFilePath);
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

bool AdvUserFirmwareSegmentFlash::firmwareFlashCheck()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    bool ok = true;
    if(d->m_devList.count() == 0){
        QMessageBox::warning(this, tr("Warning!"), tr("No device connected!"), QMessageBox::Ok);
        ok = false;
    }
    if(ok){
        if(!d->m_crtDev->isConnecting()){
            QMessageBox::warning(this, tr("Warning!"), tr("Device:%1 does not connect!").arg(d->m_crtDev->modelName()), QMessageBox::Ok);
            ok = false;
        }
    }
    if(ok){
        for(quint16 i = 0; i < d->m_crtDev->axisNum(); i++){
            if(d->m_crtDev->axisServoIsOn(i))
                ok = false;
        }
        if(!ok)
            QMessageBox::warning(this,tr("Warning!"), tr("Axis of the current device is serving!"), QMessageBox::Ok);
    }
    return ok;
}

void AdvUserFirmwareSegmentFlash::onActionComboBoxIndexChange(int index)
{
    Q_D(AdvUserFirmwareSegmentFlash);
    d->m_crtDev = d->m_devList.at(index);
}

void AdvUserFirmwareSegmentFlash::onActionDSPToolBtnClicked()
{
    Q_D(AdvUserFirmwareSegmentFlash);
    QString hexFilePath = QFileDialog::getOpenFileName(this, tr("Please select one HEX type file!"), d->m_openFilePath, tr("HEX File(*.hex)"));
    bool ok = true;
    if(hexFilePath.compare("")==0){
        QMessageBox::warning(this, tr("Warning!"), tr("NO file selected!"), QMessageBox::Ok);
        ok = false;
    }
    if(ok){
        QDir dir(hexFilePath);
        ok = dir.cdUp();
        d->m_openFilePath = dir.absolutePath();

        d->m_hexFilePath = hexFilePath;
        ui->plainTextEdit->appendPlainText(tr("Selected HEX type file:\n%1\n").arg(d->m_hexFilePath));
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
    QString rpdFilePath = QFileDialog::getOpenFileName(this, tr("Please select one RPD type file!"), d->m_openFilePath, tr("RPD File(*.rpd)"));
    bool ok = true;
    if(rpdFilePath.compare("")==0){
        QMessageBox::warning(this, tr("Warning!"), tr("NO file selected!"), QMessageBox::Ok);
        ok = false;
    }
    if(ok){
        QDir dir(rpdFilePath);
        ok = dir.cdUp();
        d->m_openFilePath = dir.absolutePath();

        d->m_rpdFilePath = rpdFilePath;
        ui->plainTextEdit->appendPlainText(tr("Selected RPD type file:\n%1\n").arg(d->m_rpdFilePath));
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
    QStringList xmlFilePaths = QFileDialog::getOpenFileNames(this, tr("Please select three XML type files!"), d->m_openFilePath, tr("XML File(*.xml)"));
    QFileInfo fileinfo;
    bool ok = true;
    if(xmlFilePaths.count() == 0){
        QMessageBox::warning(this, tr("Warning!"), tr("NO file selected!"), QMessageBox::Ok);
        ok = false;
    }
//    if(xmlFilePaths.count() != 3)
//    {
//        QMessageBox::warning(this, tr("Warning!"), tr("The number of documents selected is not three;\nPlease select again!"), QMessageBox::Ok);
//        ok = false;
//    }
//    if(ok){
//        char *fileFlag = new char[3];
//        memset(fileFlag,0,3);
//        QString str = "The following files are missing from the selected file :";
//        for(int i = 0; i < 3; i++){
//            fileinfo.setFile(xmlFilePaths.at(i));
//            if(fileinfo.fileName().compare("FILENAME_XML_FLASHPRM.xml") == 0)
//                fileFlag[0] += 1;
//            if(fileinfo.fileName().compare("FILENAME_XML_RAMPRM0.xml") == 0)
//                fileFlag[1] += 1;
//            if(fileinfo.fileName().compare("FILENAME_XML_RAMPRM1.xml") == 0)
//                fileFlag[2] += 1;
//        }
//        if(fileFlag[0] == 0||fileFlag[1] == 0||fileFlag[2] == 0){
//            if(fileFlag[0] == 0)
//                str += "\n\tFILENAME_XML_FLASHPRM.xml;";
//            if(fileFlag[1] == 0)
//                str += "\n\tFILENAME_XML_RAMPRM0.xml;";
//            if(fileFlag[2] == 0)
//                str += "\n\tFILENAME_XML_RAMPRM1.xml;";
//            QMessageBox::warning(this, tr("Warning!"), str+"\nPlease select again!", QMessageBox::Ok);
//            ok = false;
//        }
//        delete fileFlag;
//    }
    if(ok){
        QDir dir(xmlFilePaths.at(0));
        ok = dir.cdUp();
        d->m_openFilePath = dir.absolutePath();

        d->m_xmlFilePaths = xmlFilePaths;
        for(int i=0; i < xmlFilePaths.count(); i++){
            fileinfo.setFile(d->m_xmlFilePaths.at(i));
            if(i==0){
                ui->plainTextEdit->appendPlainText(tr("Selected XML type files:\n(%1)%2\n").arg(i+1).arg(d->m_xmlFilePaths.at(i)));
                ui->lineEditFLASH->setText(fileinfo.fileName() + ";");
            }else{
                ui->plainTextEdit->insertPlainText(tr("(%1)%2\n").arg(i+1).arg(d->m_xmlFilePaths.at(i)));
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
    bool ok = firmwareFlashCheck();
    if(ok){
        int dspNum = 0;
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
        emit startDownload(false);
        for(int i = 0; i < dspNum; i++){
            qint16 ret = d->m_crtDev->socketCom()->downLoadDSPFLASH(i, d->m_hexFilePath.toStdWString(), updateProgressBar, (void *)ui->progressBar);
            ui->plainTextEdit->appendPlainText(tr("Downloading dsp:%1").arg(i+1));
            if(ret != 0){
                ui->plainTextEdit->appendPlainText(tr("Download dsp:%1 fail!").arg(i+1));
                ok = false;
            }
        }
        emit startDownload(true);
    }
    if(ok){
        ui->plainTextEdit->appendPlainText(tr("Download dsp successful!"));
    }else{
        ui->plainTextEdit->appendPlainText(tr("Download dsp fail!"));
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
    bool ok = firmwareFlashCheck();
    if(ok){
        int fpgaNum = 0;
        int fpgaAxisNum = 0;
        QString filePath = GTUtils::sysPath() + d->m_crtDev->typeName() + "/" + d->m_crtDev->modelName() + "/"\
                            + d->m_crtDev->versionName() + "/" + "PrmFirmwareUpdate.xml";
        qDebug()<<"filePath"<<filePath;
        QTreeWidget *tree = QtTreeManager::createTreeWidgetFromXmlFile(filePath);
        QTreeWidgetItem *fpgaNameItem = GTUtils::findItem("fpga", tree, 0);
        if(fpgaNameItem != NULL){
            fpgaNum = fpgaNameItem->text(1).toInt();
            fpgaAxisNum = fpgaNameItem->child(0)->text(1).toInt();
        }else{
            fpgaNum = 1;
            fpgaAxisNum = d->m_crtDev->axisNum();
        }
        delete tree;
        emit startDownload(false);
        for(int i = 0; i < fpgaNum; i++){
            qDebug()<<"path"<<d->m_rpdFilePath;
            qDebug()<<"1"<<i * fpgaAxisNum;
            qint16 ret = d->m_crtDev->socketCom()->downLoadFPGAFLASH(i * fpgaAxisNum, d->m_rpdFilePath.toStdWString(), updateProgressBar, (void *)ui->progressBar);
            if(ret != 0){
                ok = false;
            }
        }
        emit startDownload(true);
    }
    if(ok){
        ui->plainTextEdit->appendPlainText(tr("Download fpga successful!"));
    }else{
        ui->plainTextEdit->appendPlainText(tr("Download fpga fail!"));
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
    bool ok = firmwareFlashCheck();
    emit startDownload(false);
    if(ok){
        quint8 axis = 0;
        short value = ui->progressBar->value();
        QList<int> fileTypeList;
        fileTypeList<<0<<0<<0;
        ok = d->m_crtDev->writeXml(axis, d->m_xmlFilePaths, fileTypeList, d->m_xmlFilePaths.length(), updateProgressBar, (void *)ui->progressBar, value);        
    }
    emit startDownload(true);
    if(ok){
        ui->plainTextEdit->appendPlainText(tr("Download flash successful!"));
    }else{
        ui->plainTextEdit->appendPlainText(tr("Download flash fail!"));
    }
    ui->progressBar->setVisible(false);
    ui->progressBar->setValue(0);
    qApp->processEvents();
}


