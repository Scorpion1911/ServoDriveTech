#include "firmwaresegmentflashdialog.h"
#include "ui_firmwaresegmentflashdialog.h"
#include "ServoDriverComDef.h"
#include "ServoDriverComDll.h"
#include <QProgressBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include "linksocket.h"

FirmwareSegmentFlashDialog::FirmwareSegmentFlashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirmwareSegmentFlashDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint|Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
    uiInit();

    QObject::connect(ui->connectBtn_firm,SIGNAL(clicked()),this,SLOT(onActionConnectClicked()));
    QObject::connect(ui->disconnectBtn_firm_2,SIGNAL(clicked()),this,SLOT(onActionDisconnectClicked()));

    QObject::connect(ui->toolBtn_firmDSP_6,SIGNAL(clicked()),this,SLOT(onActionDSPToolBtnClicked()));
    QObject::connect(ui->toolBtn_firmFPGA_6,SIGNAL(clicked()),this,SLOT(onActionFPGAToolBtnClicked()));
    QObject::connect(ui->toolBtn_firmFLASH_6,SIGNAL(clicked()),this,SLOT(onActionFLASHToolBtnClicked()));

    QObject::connect(ui->btn_firmflashDSP_6,SIGNAL(clicked()),this,SLOT(onActionDSPFlashBtnClicked()));
    QObject::connect(ui->btn_firmflashFPGA_6,SIGNAL(clicked()),this,SLOT(onActionFPGAFlashBtnClicked()));
    QObject::connect(ui->btn_firmflashFLASH_6,SIGNAL(clicked()),this,SLOT(onActionFLASHFlashBtnClicked()));
}

FirmwareSegmentFlashDialog::~FirmwareSegmentFlashDialog()
{
    delete ui;
}

void FirmwareSegmentFlashDialog::uiInit()
{
    ui->progressBar_firm_6->setValue(0);
    ui->progressBar_firm_6->setVisible(false);
    ui->textEdit_firm_6->clear();
    ui->textEdit_firm_6->appendPlainText(tr("Please connect device!"));
    ui->btn_firmflashDSP_6->setEnabled(false);
    ui->btn_firmflashFPGA_6->setEnabled(false);
    ui->btn_firmflashFLASH_6->setEnabled(false);
}

void FirmwareSegmentFlashDialog::updateProgressBar(void *arg,qint16 *value)
{
    QProgressBar *progressBar = static_cast<QProgressBar *>(arg);
    progressBar->setValue(*value);
    qApp->processEvents();
}

void FirmwareSegmentFlashDialog::onActionConnectClicked()
{
    COM_ERROR error = COM_OK;
    ui->progressBar_firm_6->setVisible(true);
    error = static_cast<COM_ERROR>(GTSD_CMD_Open(updateProgressBar, (void*)ui->progressBar_firm_6,1));

    if(error == COM_OK){
        ui->textEdit_firm_6->appendPlainText(tr("Connect device succeed!"));
        ui->btn_firmflashDSP_6->setEnabled(true);
        ui->btn_firmflashFPGA_6->setEnabled(true);
        ui->btn_firmflashFLASH_6->setEnabled(true);

        int16 ret = GTSD_CMD_GetStationAxisNum(&m_axisNum,1,1);
        if(ret != 0){
            ui->textEdit_firm_6->appendPlainText(tr("Get axis num false!"));
        }       
    }else{
        GTSD_CMD_Close(GTSD_COM_TYPE_RNNET);
        ui->textEdit_firm_6->appendPlainText(tr("Connect device failed!"));
        QMessageBox::warning(this,tr("Warning"),tr("Connect device failed!"),QMessageBox::Ok);
    }
    ui->progressBar_firm_6->setVisible(false);
    ui->progressBar_firm_6->setValue(0);
    qApp->processEvents();
}

void FirmwareSegmentFlashDialog::onActionDisconnectClicked()
{
    GTSD_CMD_Close(GTSD_COM_TYPE_RNNET);
    ui->textEdit_firm_6->appendPlainText(tr("Disconnect device!"));
    ui->btn_firmflashDSP_6->setEnabled(false);
    ui->btn_firmflashFPGA_6->setEnabled(false);
    ui->btn_firmflashFLASH_6->setEnabled(false);
}

void FirmwareSegmentFlashDialog::onActionDSPToolBtnClicked()
{
    m_hexFilePath = QFileDialog::getOpenFileName(this,tr("Please select one HEX type file!"),"C:/Users/googol/Desktop",tr("HEX File(*.hex)"));
    ui->textEdit_firm_6->appendPlainText(tr("Selected HEX type file:\n\t%1").arg(m_hexFilePath));
    QFileInfo fileinfo;
    fileinfo.setFile(m_hexFilePath);
    ui->lineEdit_firmDSP_6->setText(fileinfo.fileName());
}

void FirmwareSegmentFlashDialog::onActionFPGAToolBtnClicked()
{
    m_rpdFilePath = QFileDialog::getOpenFileName(this,tr("Please select one RPD type file!"),"C:/Users/googol/Desktop",tr("RPD File(*.rpd)"));
    ui->textEdit_firm_6->appendPlainText(tr("Selected RPD type file:\n\t%1").arg(m_rpdFilePath));
    QFileInfo fileinfo;
    fileinfo.setFile(m_rpdFilePath);
    ui->lineEdit_firmFPGA_6->setText(fileinfo.fileName());
}

void FirmwareSegmentFlashDialog::onActionFLASHToolBtnClicked()
{
    QStringList xmlFilePaths = QFileDialog::getOpenFileNames(this,tr("Please select three XML type files!"),"C:/Users/googol/Desktop",tr("XML File(*.xml)"));
    QFileInfo fileinfo;
    bool ok = true;
    if(xmlFilePaths.count()!=3)
    {
        QMessageBox::warning(this,tr("Warning!"),tr("The number of documents selected is not three;\nPlease select again!"),QMessageBox::Ok);
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
            QMessageBox::warning(this,tr("Warning!"),str+"\nPlease select again!",QMessageBox::Ok);
            ok = false;
        }
    }
    if(ok){
        m_xmlFilePaths = xmlFilePaths;
        for(int i=0; i < 3; i++){
            fileinfo.setFile(m_xmlFilePaths.at(i));
            if(i==0){
                ui->textEdit_firm_6->appendPlainText(tr("Selected XML type files:\n\t(%1)%2").arg(i+1).arg(m_xmlFilePaths.at(i)));
                ui->lineEdit_firmFLASH_6->setText(fileinfo.fileName()+";");
            }else{
                ui->textEdit_firm_6->appendPlainText(tr("\t(%1)%2").arg(i+1).arg(m_xmlFilePaths.at(i)));
                ui->lineEdit_firmFLASH_6->insert(fileinfo.fileName()+";");
            }
        }
    }
//    ui->textEdit_firm_6->appendPlainText(tr("Selected *.xml file:(1)%1;\n(2)%2;\n(3)%3").arg(m_xmlFilePaths.at(0)).arg(m_xmlFilePaths.at(1).arg(m_xmlFilePaths.at(2))));
}

void FirmwareSegmentFlashDialog::onActionDSPFlashBtnClicked()
{
    ui->progressBar_firm_6->setVisible(true);
    ui->textEdit_firm_6->appendPlainText(tr("Start download dsp!"));
    bool ok = true;
    int dspNum = 0;
    dspNum = (m_axisNum+1)/2;
    for(int i = 0; i < dspNum; i++){
        int ret = GTSD_CMD_ProcessorFlashHandler(i*2,m_hexFilePath.toStdWString(),updateProgressBar,ui->progressBar_firm_6,1,1);
        if(ret != 0){
            ui->textEdit_firm_6->appendPlainText(tr("Download dsp:%1 fail!").arg(i+1));
            ok = false;
        }
    }
    if(ok){
        ui->textEdit_firm_6->appendPlainText(tr("Download dsp successful!"));
    }
    ui->progressBar_firm_6->setVisible(false);
    ui->progressBar_firm_6->setValue(0);
    qApp->processEvents();
}

void FirmwareSegmentFlashDialog::onActionFPGAFlashBtnClicked()
{
    ui->progressBar_firm_6->setVisible(true);
    ui->textEdit_firm_6->appendPlainText(tr("Start download fpga!"));
    bool ok = true;
    int ret = GTSD_CMD_ProcessorFlashHandler(m_axisNum,m_rpdFilePath.toStdWString(),updateProgressBar,ui->progressBar_firm_6,1,1);
    if(ret != 0){
        ui->textEdit_firm_6->appendPlainText(tr("Download fpga fail!"));
        ok = false;
    }
    if(ok){
       ui->textEdit_firm_6->appendPlainText(tr("Download fpga successful!"));
    }
    ui->progressBar_firm_6->setVisible(false);
    ui->progressBar_firm_6->setValue(0);
    qApp->processEvents();
}

void FirmwareSegmentFlashDialog::onActionFLASHFlashBtnClicked()
{
    ui->progressBar_firm_6->setVisible(true);
    ui->textEdit_firm_6->appendPlainText(tr("Start download flash!"));
    bool ok = true;
    quint8 axis = 0;
    short value = ui->progressBar_firm_6->value();
    //QList<int> fileTypeList;
    //fileTypeList<<0<<0<<0;
   // LinkSocket linksocket;   
   // bool ok = linksocket.writeXml(axis,m_xmlFilePaths,fileTypeList,m_xmlFilePaths.length(),updateProgressBar,ui->progressBar_firm_6,value);

    char *xmlFilePaths[3];
    int fileType[3] = {0};
    for(int i = 0; i < 3; i++){
        QString str = m_xmlFilePaths.at(i);
        int size = str.length();
        char *path = (char*)malloc(size+1);
        memset(path,0,size+1);
        memcpy_s(path,size,str.toStdString().c_str(),size);
        xmlFilePaths[i] = path;
    }

    short ret = GTSD_CMD_XmlWriteFile(axis,xmlFilePaths,fileType,m_xmlFilePaths.length(),updateProgressBar,ui->progressBar_firm_6,value,1,1);
    if(ret != 0){
        ui->textEdit_firm_6->appendPlainText(tr("Download flash fail!"));
        ok = false;
    }
    if(ok){
        ui->textEdit_firm_6->appendPlainText(tr("Download flash successful!"));
    }
    ui->progressBar_firm_6->setVisible(false);
    ui->progressBar_firm_6->setValue(0);
    qApp->processEvents();
}
