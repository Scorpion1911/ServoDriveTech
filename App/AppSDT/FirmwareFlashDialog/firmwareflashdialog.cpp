#include "firmwareflashdialog.h"
#include "ui_firmwareflashdialog.h"
#include "FolderCompressor.h"
#include "sevdevice.h"
#include "gtutils.h"
#include "qttreemanager.h"
#include "dbmanager.h"
#include "deviceidhelper.h"
#include "optpath.h"
#include "optautoload.h"
#include "optcontainer.h"
#include "servofile.h"

#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressBar>
#include <QTextStream>
#include <QDebug>

#define FILENAME_XML_FLASHPRM "FlashPrm_AllAxis.xml"
#define FILENAME_XML_RAMPRM0 "PrmRAMAxis0.xml"
#define FILENAME_XML_RAMPRM1 "PrmRAMAxis1.xml"
#define FPGA_NAME "fpga"
#define DSP_NAME "dsp"
#define FILENAME_XML_FUNCEXTENSION "PrmFirmwareUpdate.xml"

#define XMLFILE_ROW_INDEX 0
#define XMLFILE_CHILD_VERSION_ROW_INDEX 0
#define XMLFILE_NODE_NAME "XmlFileInformation"

static bool m_erasing;
static bool m_downloading;

FirmwareFlashDialog::FirmwareFlashDialog(QList<SevDevice *> &devList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FirmwareFlashDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
    m_devList = devList;
    uiInit();
    m_filePath = ".";
    OptPath *optpath = dynamic_cast<OptPath *>(OptContainer::instance()->optItem("optpath"));
    if (optpath != NULL) {
        m_filePath = optpath->flashFilePath();
    }
    m_desPath = m_filePath + "/DecompressdFiles";
    m_barCount = 0;
    m_boxCount = 0;
//    QDir dir(m_desPath);
//    if (dir.exists()) {
//        deleteDir(m_desPath);
//    }
//    dir.mkdir(m_desPath);
    createConnections();
}

FirmwareFlashDialog::~FirmwareFlashDialog()
{
    delete ui;
}

void FirmwareFlashDialog::uiInit()
{
    ui->progressBar_firm->setVisible(false);
    ui->widget_firm->setVisible(false);
    QVBoxLayout *vBox = new QVBoxLayout;
    for (int i = 0; i < m_devList.length(); i++) {
        bool hasNickName = true;
        QString prefix;
//        prefix = hasNickName?tr("[%1] ").arg(m_devList.at(i)->aliasName()):"[0]";
        prefix = tr("[%1] ").arg(m_devList.at(i)->aliasName());
        QCheckBox *box = new QCheckBox(prefix + m_devList.at(i)->modelName());
        m_boxList.append(box);
        vBox->addWidget(box);
        connect(box, SIGNAL(clicked(bool)), this, SLOT(onSingleBoxClicked(bool)));
    }
    vBox->setSpacing(2);
    ui->scrollAreaWidgetContents->setLayout(vBox);
    connect(ui->checkBox_all, SIGNAL(clicked(bool)), this, SLOT(onAllBoxClicked(bool)));
}

void FirmwareFlashDialog::createConnections()
{
    connect(ui->toolBtn_firm, SIGNAL(clicked()), this, SLOT(onActnToolbtnClicked()));
    connect(ui->btn_firmFlash, SIGNAL(clicked()), this, SLOT(onActnFlashBtnClicked()));
    connect(this, SIGNAL(sendProcessMsg(int, QString)), this, SLOT(receiveProcessMsg(int, QString)));
}

void FirmwareFlashDialog::processCallBack(void *argv, short *value)
{
    QProgressBar *pBar = static_cast<QProgressBar *>(argv);
    pBar->setValue(*value);
    qApp->processEvents();
}

void FirmwareFlashDialog::updateProgressValueFPGA(void *arg, qint16 *value)
{
    bool isErase;
    quint16 bitHigh = (quint16)(1<<15);
    isErase = (*value) &bitHigh;
    qint16 v = (*value)&(~bitHigh);
    FirmwareFlashDialog *dialog = static_cast<FirmwareFlashDialog *>(arg);
    QProgressBar *bar = dialog->ui->progressBar_firm;
    QPlainTextEdit *label = dialog->ui->infoDisplay_firm;
    bar->setValue(v % 100);
    if (isErase && m_erasing) {
        m_erasing = false;
        label->appendPlainText(tr("FPGA Erasing ......"));
    } else if (!isErase && !m_erasing && m_downloading){
        m_downloading = false;
        label->appendPlainText(tr("FPGA DownLoad ......"));
    }
    qApp->processEvents();
}

void FirmwareFlashDialog::closeEvent(QCloseEvent *event)
{
    QDir dir(m_desPath);
    if (dir.exists()) {
        deleteDir(m_desPath);
    }
    event->accept();
}

QStringList FirmwareFlashDialog::getFilesFromExt(const QString &fileExt, const QString &filePath, int num)
{
    int count = 0;
    QStringList result;
    QDir dir(filePath);
    if (!dir.exists()){
        return result;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    for (int i = 0; i < fileList.count(); i++) {
        QFileInfo file = fileList.at(i);
        QString ext = file.suffix();
        if (ext.compare(fileExt) == 0) {
            count++;
            result.append(file.filePath());
        }
        if (count >= num) {
            break;
        }
    }
    return result;
}

QString FirmwareFlashDialog::getFileFromName(const QString &fileName, const QString &filePath)
{
    QDir dir(filePath);
    if (!dir.exists()){
        return "";
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    for (int i = 0; i < fileList.count(); i++) {
        QFileInfo file = fileList.at(i);
        QString name = file.fileName();
        if (name.compare(fileName) == 0) {
            return name;
        }
    }
    return "";
}

void FirmwareFlashDialog::onActnToolbtnClicked()
{
    ui->lineEdit_firm->clear();
    ui->infoDisplay_firm->clear();
    m_decompressPath = QFileDialog::getOpenFileName(0, tr("Open File"), m_filePath, tr("SDT Files(*.sdt)"));
    if (m_decompressPath.compare("") == 0) {
        return;
    }
    QDir dir(m_desPath);
    if (dir.exists()) {
        deleteDir(m_desPath);
    }
    dir.mkdir(m_desPath);
    FolderCompressor *folderComp = new FolderCompressor(0);
    bool ok = folderComp->decompressFolder(m_decompressPath, m_desPath);
    delete folderComp;
    if (!ok) {
        QMessageBox::warning(this, tr("Warning"), tr("Decompressing fails!"));
        deleteDir(m_desPath);
        return;
    }
    QFileInfo fileInfo;
    fileInfo.setFile(m_decompressPath);
    ui->lineEdit_firm->setText(fileInfo.fileName());
    m_filePath = m_decompressPath;
    ui->widget_firm->setVisible(true);

    QFile infoFile(m_desPath + "/infoFile.ini");
    if (!infoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("File open fails!"));
        deleteDir(m_desPath);
        return;
    }
    QTextStream in(&infoFile);
    QString line = in.readLine();
    QStringList lineList = line.split(":");
    m_dspVersion = lineList.last();
    ui->infoDisplay_firm->appendPlainText(tr("DSP Version: ") + m_dspVersion);

    line = in.readLine();
    lineList = line.split(":");
    QString hexNote = lineList.last();
    ui->infoDisplay_firm->appendPlainText(tr("DSP Note: ") + hexNote);


    line = in.readLine();
    lineList = line.split(":");
    m_fpgVersion = lineList.last();
    ui->infoDisplay_firm->appendPlainText(tr("FPGA Version: ") + m_fpgVersion);

    line = in.readLine();
    lineList = line.split(":");
    QString fpgaNote = lineList.last();
    ui->infoDisplay_firm->appendPlainText(tr("FPGA Note: ") + fpgaNote);
}

void FirmwareFlashDialog::onActnFlashBtnClicked()
{
    if (m_decompressPath.compare("") == 0) {
        return;
    }
    OptAutoLoad *optAuto=dynamic_cast<OptAutoLoad *>(OptContainer::instance()->optItem("optautoload"));
    bool needMatch = optAuto->versionMatch();
    emit startDownload(false);
    for (int i = 0; i < m_boxList.count(); i++) {
        if (!m_boxList.at(i)->isChecked()) {
            continue;
        }
        if (!m_devList.at(i)->isConnecting()) {
//---------------------------------
            return;
        }
        ui->progressBar_firm->setValue(0);
        ui->progressBar_firm->setVisible(true);

        m_curDevice = m_devList.at(i);
        m_devStr = tr("Device[%1]: ").arg(QString::number(m_curDevice->stationId()));
        DeviceIdHelper *idHelper = new DeviceIdHelper(m_curDevice->socketCom(), 0);
        bool ok = true;
        int servoIndex = -1;
        for (int j = 0; j < m_curDevice->axisNum(); j++) {
            if (m_curDevice->axisServoIsOn(j)) {
                ok = false;
                servoIndex = j;
                break;
            }
        }
        if (!ok) {
            ui->infoDisplay_firm->appendPlainText(m_devStr + tr("Axis%1 is on servo!").arg(servoIndex));
            delete idHelper;
            ui->progressBar_firm->setVisible(false);
            return;
        }

        ui->infoDisplay_firm->appendPlainText(m_devStr + tr("1.Checking version!"));
        if (needMatch) {
            quint32 pwrId = idHelper->readPwrId(ok);
            if (!ok) {
                ui->infoDisplay_firm->appendPlainText(tr("Reading powerboard Id fails!"));
                QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Warning"), tr("Reading powerboard Id fails! Are you sure to continue?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if (!(rb == QMessageBox::Yes)) {
                    delete idHelper;
                    ui->progressBar_firm->setVisible(false);
                    return;
                }
            }
            QString pwrStr = QString::number(pwrId);

            ok = true;
            quint32 ctrId = idHelper->readCtrId(ok);
            if (!ok) {
                ui->infoDisplay_firm->appendPlainText(tr("Reading controlboard Id fails!"));
                QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Warning"), tr("Reading controlboard Id fails! Are you sure to continue?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if (!(rb == QMessageBox::Yes)) {
                    delete idHelper;
                    ui->progressBar_firm->setVisible(false);
                    return;
                }
            }
            QString ctrStr = QString::number(ctrId);
            delete idHelper;

            ok = true;
            DBManager *dbManager = new DBManager(GTUtils::databasePath() + "Version/", "root", "");
            QStringList verList;
            verList<<ctrStr<<m_dspVersion<<m_fpgVersion<<pwrStr;
            ok = dbManager->checkValid(verList);
            qDebug()<<"aa";
            delete dbManager;
            if (!ok) {
                QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Warning"), tr("Version do not match. Are you sure to continue?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                if (!(rb == QMessageBox::Yes)) {
                    ui->progressBar_firm->setVisible(false);
                    return;
                }
            }
        }
        quint16 dspVersion;
        qint16 err = m_curDevice->socketCom()->readDSPVersion(0, dspVersion);
        qDebug()<<"dsp ver"<<dspVersion;
        if (err != 0) {
            ui->infoDisplay_firm->appendPlainText(tr("Read dsp version error!"));
            return;
        }
        QString realDspVersion = QString::number(dspVersion);
        ok = true;
        if (m_dspVersion.compare(realDspVersion) != 0) {
            qDebug()<<"up"<<m_dspVersion;
            qDebug()<<"real"<<realDspVersion;
            ok = getParaFiles();
        }
        if (!ok) {
            //ui->infoDisplay_firm->appendPlainText(tr("Parameter restoring fails!"));
            //deleteDir(m_desPath);
            ui->progressBar_firm->setVisible(false);
            return;
        }
        ok = true;
        qDebug()<<"b";
        if (ui->checkBox_firmHex->isChecked()) {
            ok = downloadHexFile();
        }
        qDebug()<<"c";
        if (!ok) {
            ui->infoDisplay_firm->appendPlainText(tr("Downloading hex file fails!"));
            //deleteDir(m_desPath);
            ui->progressBar_firm->setVisible(false);
            return;
        }
        ok = true;
        if (ui->checkBox_firmRpd->isChecked()) {
            ok = downloadRpdFile();
        }
        if (!ok) {
            ui->infoDisplay_firm->appendPlainText(tr("Downloading rpd file fails!"));
            //deleteDir(m_desPath);
            ui->progressBar_firm->setVisible(false);
            return;
        }
        ok = true;
        if (ui->checkBox_firmXml->isChecked()) {
            ok = downloadXmlFiles();
        }
        if (!ok) {
            ui->infoDisplay_firm->appendPlainText(tr("Downloading xml files fail!"));
            //deleteDir(m_desPath);
            ui->progressBar_firm->setVisible(false);
            return;
        }
    }
    emit startDownload(true);
    ui->infoDisplay_firm->appendPlainText(m_devStr + tr("Downloading succeeds!"));
    //deleteDir(m_desPath);
    ui->progressBar_firm->setVisible(false);
}

void FirmwareFlashDialog::receiveProcessMsg(int value, QString msg)
{
    Q_UNUSED(msg);
    qApp->processEvents();
    ui->progressBar_firm->setValue(value);
    //ui->infoDisplay_firm->appendPlainText(msg);
}

void FirmwareFlashDialog::onActnComboBoxIndexChanged(int index)
{
    DeviceIdHelper *idHelper = new DeviceIdHelper(m_devList.at(index)->socketCom(), 0);
    bool ok;
    QString fpgaId = idHelper->readFpgaId(ok);
    delete idHelper;
    if (!ok) {
        qDebug()<<"read err";
        return;
    }
    if (fpgaId.compare(m_fpgVersion) == 0) {
        ui->checkBox_firmRpd->setChecked(false);
    } else {
        ui->checkBox_firmRpd->setChecked(true);
    }
}

void FirmwareFlashDialog::onAllBoxClicked(bool checked)
{
    for (int i = 0; i < m_devList.count(); i++) {
        m_boxList.at(i)->setChecked(checked);
    }
    if (checked) {
        m_boxCount = m_devList.count();
    } else {
        m_boxCount = 0;
    }
}

void FirmwareFlashDialog::onSingleBoxClicked(bool checked)
{
    if (checked) {
        m_boxCount++;
    } else {
        m_boxCount--;
    }
    if (m_boxCount == m_devList.count()) {
        ui->checkBox_all->setChecked(true);
    } else {
        ui->checkBox_all->setChecked(false);
    }
}

bool FirmwareFlashDialog::deleteDir(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    bool ok;
    for (int i = 0; i < fileList.count(); i++) {
        QFileInfo file = fileList.at(i);
        ok = file.dir().remove(file.fileName());
        if (!ok) {
            return ok;
        }
    }
    ok = dir.rmdir(dir.absolutePath());
    return ok;
}

bool FirmwareFlashDialog::getParaFiles()
{
    ServoFile* paraManager = new ServoFile;
    connect(paraManager, SIGNAL(sendProgressbarMsg(int,QString)), this, SLOT(receiveProcessMsg(int,QString)));
    ui->infoDisplay_firm->appendPlainText(tr("Restoring parameters!"));
    QString xmlPath = m_desPath + "/oldPara.xml";
    bool ok = paraManager->upLoadFile(processCallBack, ui->progressBar_firm, xmlPath, m_curDevice);
    if (!ok) {
        ui->infoDisplay_firm->appendPlainText(tr("Restoring parameters fail!"));
        delete paraManager;
        return false;
    }
    QTreeWidget* restoreTree = QtTreeManager::createTreeWidgetFromXmlFile(xmlPath);
    QTreeWidget* moduleTree = QtTreeManager::createTreeWidgetFromXmlFile(m_desPath + "/" + FLASH_ALL_PRM_NAME);
    QTreeWidgetItem* headNode = restoreTree->topLevelItem(XMLFILE_ROW_INDEX)->child(XMLFILE_CHILD_VERSION_ROW_INDEX);
    QString headNodeName = headNode->parent()->text(GT::COL_FLASH_ALLAXIS_NAME);
    QString restoreVersion = "";
    if (headNodeName.compare(XMLFILE_NODE_NAME) == 0) {
        restoreVersion = headNode->text(GT::COL_FLASH_ALLAXIS_VALUE);
        restoreTree->takeTopLevelItem(XMLFILE_ROW_INDEX);
    }
    headNode = moduleTree->topLevelItem(XMLFILE_ROW_INDEX)->child(XMLFILE_CHILD_VERSION_ROW_INDEX);
    headNodeName = headNode->parent()->text(GT::COL_FLASH_ALLAXIS_NAME);
    if (headNodeName.compare(XMLFILE_NODE_NAME) == 0) {
        moduleTree->takeTopLevelItem(XMLFILE_ROW_INDEX);
    }
    if (restoreVersion.compare(m_dspVersion) != 0) {
        ui->infoDisplay_firm->appendPlainText(tr("Updating parameters!"));
        paraManager->updatePrmTree(restoreTree, moduleTree);
        ui->infoDisplay_firm->appendPlainText(tr("Downloading parameters!"));
        ok = paraManager->downLoadTree(moduleTree, m_curDevice);
        if (!ok) {
            delete restoreTree;
            delete moduleTree;
            delete paraManager;
            return false;
        }
    }
    delete restoreTree;
    delete moduleTree;
    delete paraManager;
    return true;
}

bool FirmwareFlashDialog::downloadHexFile()
{
    ui->infoDisplay_firm->appendPlainText(m_devStr + tr("2.Downloading hex file!"));
    QStringList hexList = getFilesFromExt("hex", m_desPath, 1);
    QString hexPath = hexList.at(0);
    if (!m_curDevice->isConnecting()) {
        QMessageBox::information(0, tr("Warning"), tr("please open the com first !"));
        return false;
    }
    int dspNum;
    QString filePath = GTUtils::sysPath() + m_curDevice->typeName() + "/" + m_curDevice->modelName() + "/" \
                       + m_curDevice->versionName() + "/" + FILENAME_XML_FUNCEXTENSION;
    QTreeWidget* tree = QtTreeManager::createTreeWidgetFromXmlFile(filePath);
    QTreeWidgetItem *dspItem = GTUtils::findItem(DSP_NAME, tree, GT::COL_PRMEXT_NAME);
    if (dspItem != NULL) {
        dspNum = dspItem->text(GT::COL_PRMEXT_PARA).toInt();
    } else {
        dspNum = (m_curDevice->axisNum() + 1) / 2;
    }
    delete tree;
    for (int i = 0; i < dspNum; i++) {
        ui->infoDisplay_firm->appendPlainText(tr("Downloading DSP %1.").arg(i + 1));
        qint16 ret = m_curDevice->socketCom()->downLoadDSPFLASH(i, hexPath.toStdWString(), processCallBack, (void *)ui->progressBar_firm);
        if (ret != 0) {
            return false;
        }
    }
    return true;
}

bool FirmwareFlashDialog::downloadRpdFile()
{
    if (!m_curDevice->isConnecting()) {
        QMessageBox::information(0, tr("Warning"), tr("please open the com first !"));
        return false;
    }
    ui->infoDisplay_firm->appendPlainText(m_devStr + tr("3. Downloading rpd file!"));
    QStringList rpdList = getFilesFromExt("rpd", m_desPath, 1);
    qDebug()<<"rpd count"<<rpdList.count();
    QStringList binList = getFilesFromExt("bin", m_desPath, 1);
    qDebug()<<"bin count"<<binList.count();
    QString rpdPath;
    if (rpdList.count() > 0) {
        rpdPath = rpdList.at(0);
    } else if (binList.count() > 0) {
        rpdPath = binList.at(0);
    } else {
        ui->infoDisplay_firm->appendPlainText(m_devStr + tr("Cannot find FPGA firmware"));
        return false;
    }
    int fpgNum;
    int fpgAxis;
    QString filePath = GTUtils::sysPath() + m_curDevice->typeName() + "/" + m_curDevice->modelName() + "/" \
                       + m_curDevice->versionName() + "/" + FILENAME_XML_FUNCEXTENSION;
    QTreeWidget* tree = QtTreeManager::createTreeWidgetFromXmlFile(filePath);
    QTreeWidgetItem *fpgItem = GTUtils::findItem(FPGA_NAME, tree, GT::COL_PRMEXT_NAME);
    if (fpgItem != NULL) {
        fpgNum = fpgItem->text(GT::COL_PRMEXT_PARA).toInt();
        fpgAxis= fpgItem->child(0)->text(GT::COL_PRMEXT_PARA).toInt();
    } else {
        fpgNum = 1;
        fpgAxis = m_curDevice->axisNum();
    }
    delete tree;
    for (int i = 0; i < fpgNum; i++) {
        qDebug()<<"rpdPath"<<rpdPath;
        qDebug()<<"1"<<i * fpgAxis;
        m_erasing = true;
        m_downloading = true;
        qint16 ret = m_curDevice->socketCom()->downLoadFPGAFLASH(i * fpgAxis, rpdPath.toStdWString(), updateProgressValueFPGA, (void *)(this));
        m_erasing = false;
        m_downloading = false;
        if (ret != 0) {
            return false;
        }
    }
    return true;
}

bool FirmwareFlashDialog::downloadXmlFiles()
{
    if (!m_curDevice->isConnecting()) {
        QMessageBox::information(0, tr("Warning"), tr("please open the com first !"));
        return false;
    }
    ui->infoDisplay_firm->appendPlainText(m_devStr + tr("4. Downloading xml files!"));
    QStringList fileNameList;
    fileNameList.append(m_desPath + "/" + FILENAME_XML_FLASHPRM);
    fileNameList.append(m_desPath + "/" + FILENAME_XML_RAMPRM0);
    fileNameList.append(m_desPath + "/" + FILENAME_XML_RAMPRM1);
    QList<int> fileTypeList;
    fileTypeList<<0<<0<<0;
    quint8 axis = 0;
    short value = ui->progressBar_firm->value();
    bool ok = m_curDevice->writeXml(axis, fileNameList, fileTypeList, fileNameList.length(), processCallBack, ui->progressBar_firm, value);
    return ok;
}
