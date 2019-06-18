#include "parafileconversion.h"
#include "ui_parafileconversion.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTreeWidgetItemIterator>
#include <QHeaderView>
#include <QDebug>
#include <QDateTime>


#include "../../Libs/GTUtils/QtTreeManager/qttreemanager.h"
#include "gtutils.h"


#define MEMBER_NAME_AT_INPUT_TREE 0
#define MEMBER_VALUE_AT_INPUT_TREE 1
#define MEMBER_TYPE_AT_INPUT_TREE 2
#define XMLVERSION_AT_INPUT_TREE 1
#define ADDRESS_OFFSET_AT_INPUT_TREE 3
#define ADDRESS_OFFSET_AT_OUTPUT_STRUCT 5
#define SERIAL_NUM_AT_OUTPUT_TREE 0
#define OBJECT_WORD_AT_OUTPUT_TREE 1
#define MEMBER_NAME_AT_OUTPUT_TREE 2
#define MEMBER_VALUE_AT_OUTPUT_TREE 3
#define MEMBER_TYPE_AT_OUTPUT_TREE 4
#define ADDRESS_OFFSET_AT_OUTPUT_TREE 5

#define UPGRADE_FILE_TYPE_HEAD 0
#define UPGRADE_FILE_TYPE_XML 1
#define RETURN_AND_NEWLINE 2

ParaFileConversion::ParaFileConversion(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParaFileConversion)
{
    ui->setupUi(this);
    init();
}

ParaFileConversion::~ParaFileConversion()
{
    delete ui;
}

void ParaFileConversion::init()
{
    connect(ui->pushBtn_OpenFile, SIGNAL(clicked()), this, SLOT(onPushBtnOpenFileClicked()));
//    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
    connect(ui->pushbtn_ExportFile, SIGNAL(clicked()), this, SLOT(onPushBtnExprotFileClicked()));
    connect(ui->checkBox_Upgrade, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxUpgradeToggled(bool)));
    connect(ui->checkBox_HeadFile, SIGNAL(toggled(bool)), this, SLOT(onCheckBoxOutputHeadFileToggled(bool)));
    connect(ui->pushBtn_OpenFile_Upgrade, SIGNAL(clicked()), this, SLOT(onPushBtnOpenFileUpgradeClicked()));
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->pushbtn_ExportFile->setEnabled(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_memberType.append("int16");
    m_memberType.append("Uint16");
    m_memberType.append("int32");
    m_memberType.append("Uint32");
    ui->stackedWidget->setCurrentIndex(0);
//    ui->lineEdit_StrName->setEnabled(false);
    m_savePath = ".";
    m_openFilePrepare = false;
    m_upgradeFilePrepare = false;
    m_saveTreeWidget = new QTreeWidget;
    m_paraSum = 0;
}

void ParaFileConversion::printStructMemberToText(QTreeWidgetItem *treeWidgetItem, QTextStream &out)
{
    for(int i = 0; i<treeWidgetItem->childCount(); i++){
        if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_AT_INPUT_TREE).compare("-1") == 0){
        }else{
            out << "\t" << "{" << QString::number(m_memberSerialNum) << "," \
                                << "0" << "," \
                                << "\"" << treeWidgetItem->child(i)->text(MEMBER_NAME_AT_INPUT_TREE) << "\"" << ","\
                                << treeWidgetItem->child(i)->text(MEMBER_VALUE_AT_INPUT_TREE) << "," ;
            int k;
            for(k = 0; k<m_memberType.count(); k++){
                if(treeWidgetItem->child(i)->text(MEMBER_TYPE_AT_INPUT_TREE).compare(m_memberType.at(k)) == 0)
                    break;
            }
            if(k<m_memberType.count()){
                out << treeWidgetItem->child(i)->text(MEMBER_TYPE_AT_INPUT_TREE) << "," ;
            }
            else{
                out << "Uint16" << "," ;
            }
            out << treeWidgetItem->child(i)->text(ADDRESS_OFFSET_AT_INPUT_TREE) << "}," << endl;

            m_memberSerialNum++;
        }
        if(treeWidgetItem->child(i)->childCount() !=0 ){
            printStructMemberToText(treeWidgetItem->child(i), out);
        }
    }
}

void ParaFileConversion:: setEnabledExprotPushBtn()
{
    if(ui->checkBox_Upgrade->isChecked()){
        if(m_openFilePrepare && m_upgradeFilePrepare){
            ui->pushbtn_ExportFile->setEnabled(true);
        }else{
            ui->pushbtn_ExportFile->setEnabled(false);
        }
    }else{
        if(m_openFilePrepare){
            ui->pushbtn_ExportFile->setEnabled(true);
        }else{
            ui->pushbtn_ExportFile->setEnabled(false);
        }
    }
}

void ParaFileConversion::findOutIncreaseParameter(QTreeWidgetItem *treeWidgetItem)
{
    int k;
    for(int i = 0; i<treeWidgetItem->childCount(); i++){
        if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_AT_INPUT_TREE).compare("-1") != 0){
            for(k = 0; k<m_oldParaFileAddr.count(); k++){
               if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_AT_INPUT_TREE).compare(m_oldParaFileAddr.at(k)) == 0){
                 break;
               }
            }
            if(k == m_oldParaFileAddr.count()){
                QTreeWidgetItem *item = new QTreeWidgetItem;
                for(int j = 0; j<treeWidgetItem->child(i)->columnCount(); j++){
                    item->setText(j, treeWidgetItem->child(i)->text(j));
                }
                m_increaseItem.addChild(item);
                m_oldParaFileAddr.append(item->text(ADDRESS_OFFSET_AT_INPUT_TREE));
                m_paraSum++;
            }
        }
        if(treeWidgetItem->child(i)->childCount() !=0 ){
            findOutIncreaseParameter(treeWidgetItem->child(i));
        }
    }
}

void ParaFileConversion::addUpdateInfo(QTreeWidgetItem *treeWidgetItem)
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    item->setText(0, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    item->setText(1, m_xmlVersion);
    QString str;
    item->setText(2, str.setNum(m_insertNum));
    if(ui->checkBox_Upgrade->isChecked()){
        item->setText(3, str.setNum(m_paraSum));
    }else{
        item->setText(3, str.setNum(m_insertNum));
    }
    treeWidgetItem->addChild(item);
}

void ParaFileConversion::addParameterToParaItem(QTreeWidgetItem *treeWidgetItem, QTreeWidgetItem *paraItem)
{
    for(int i = 0; i<treeWidgetItem->childCount(); i++){
        if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_AT_INPUT_TREE).compare("-1") == 0){
        }else{
            QTreeWidgetItem *item = new QTreeWidgetItem;
            item->setText(SERIAL_NUM_AT_OUTPUT_TREE, QString::number(m_memberSerialNum));
            item->setText(MEMBER_NAME_AT_OUTPUT_TREE, treeWidgetItem->child(i)->text(MEMBER_NAME_AT_INPUT_TREE));
            item->setText(MEMBER_VALUE_AT_OUTPUT_TREE, treeWidgetItem->child(i)->text(MEMBER_VALUE_AT_INPUT_TREE));
            int k;
            for(k = 0; k<m_memberType.count(); k++){
                if(treeWidgetItem->child(i)->text(MEMBER_TYPE_AT_INPUT_TREE).compare(m_memberType.at(k)) == 0)
                    break;
            }
            if(k<m_memberType.count()){
                item->setText(MEMBER_TYPE_AT_OUTPUT_TREE, treeWidgetItem->child(i)->text(MEMBER_TYPE_AT_INPUT_TREE));
            }
            else{
                item->setText(MEMBER_TYPE_AT_OUTPUT_TREE, "Uint16");
            }
            item->setText(ADDRESS_OFFSET_AT_OUTPUT_TREE, treeWidgetItem->child(i)->text(ADDRESS_OFFSET_AT_INPUT_TREE));
            paraItem->addChild(item);
            m_memberSerialNum++;
        }
        if(treeWidgetItem->child(i)->childCount() !=0 ){
            addParameterToParaItem(treeWidgetItem->child(i), paraItem);
        }
    }
}


void ParaFileConversion::onPushBtnOpenFileClicked()
{
    QString OpenFilePath = QFileDialog::getOpenFileName(this, tr("Select Parameter Xml File!"), m_savePath, tr("Xml File(*.xml)"));
    if(OpenFilePath.compare("")==0){
        QMessageBox::warning(this, tr("Warning!"), tr("No file selected!"), QMessageBox::Ok);
        return ;
    }
    QDir dir(OpenFilePath);
    QString absolutePath = dir.absolutePath();
    ui->lineEdit_FilePath->setText(absolutePath);
    dir.cdUp();
    m_savePath = dir.absolutePath();

    QTreeWidget *tree = NULL;
    tree = QtTreeManager::createTreeWidgetFromXmlFile(absolutePath);
    if(tree==NULL){
        QMessageBox::warning(this, tr("Warning!"), tr("The file could not be opened!"), QMessageBox::Ok);
        return ;
    }
    if(GTUtils::findItem("SevDrvRoot0", tree, 0) != NULL){
        QTreeWidgetItem *item = NULL;
        ui->treeWidget->clear();
        item = tree->headerItem()->clone();
        ui->treeWidget->setHeaderItem(item);
        for(int i = 0; i<tree->topLevelItemCount(); i++){
            item = tree->topLevelItem(i)->clone();
            ui->treeWidget->addTopLevelItem(item);
        }
        delete tree;
        ui->treeWidget->expandAll();

        m_openFilePrepare = true;
        m_treeWidgetItem = GTUtils::findItem("SevDrvRoot0", ui->treeWidget, 0);
        QTreeWidgetItem *xmlVersionItem = GTUtils::findItem("XmlVersion", ui->treeWidget, 0);
        if(xmlVersionItem != NULL){
            m_xmlVersion = xmlVersionItem->text(XMLVERSION_AT_INPUT_TREE);
        }else{
            m_xmlVersion = "XXX";
        }
    }
    else{
        m_openFilePrepare = false;
        ui->lineEdit_FilePath->clear();
//        ui->pushbtn_ExportFile->setEnabled(false);
        QMessageBox::warning(this, tr("Warning!"), tr("This file is not the specified parameter file!\nPlease re-select!"), QMessageBox::Ok);
        return ;
    }
    setEnabledExprotPushBtn();
}

void ParaFileConversion::onPushBtnExprotFileClicked()
{
    QString str;
    if(ui->checkBox_Upgrade->isChecked()){     
        int childCount = m_increaseItem.childCount();
        for(int i = 0; i<childCount; i++){
            QTreeWidgetItem *deleteChild = m_increaseItem.child(0);
            m_increaseItem.removeChild(m_increaseItem.child(0));
            delete deleteChild;
        }
//        qDebug() << "m_increaseItem.childCount():\t" << m_increaseItem.childCount();
        findOutIncreaseParameter(m_treeWidgetItem);
        if(m_increaseItem.childCount() != 0){
            if(m_upgradeFileType == UPGRADE_FILE_TYPE_HEAD){
                QFile file(m_upgradeFilePath);
                int seekCharNum = 0;
        //        int rowNum = 0;
                if(file.open(QFile::ReadWrite | QIODevice::Text)){
                    QTextStream inOrOut(&file);
                    while(!inOrOut.atEnd()){
                        str = inOrOut.readLine();
                        if(str.contains("Struct end")){
                            break;
                        }
                      seekCharNum += (str.length() + RETURN_AND_NEWLINE);
                    }
                    inOrOut.seek(seekCharNum);
                    inOrOut << endl;

                    inOrOut << "//" << "Increase new struct member at " \
                            << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") \
                            << "\t XmlVersion: " << m_xmlVersion << endl;

                    printStructMemberToText(&m_increaseItem, inOrOut);

                    inOrOut << "}" << "//Struct end" << endl;
                    inOrOut << "#endif" << endl;
                    inOrOut << endl;
                    file.close();
                }
//                QString newHeadFile = m_upgradeFileUpPath + "/" + m_upgradeFileNameCom + m_xmlVersion + ".h";
//                QFile::rename(m_upgradeFilePath, newHeadFile);
//                m_upgradeFilePath = newHeadFile;
//                ui->lineEdit_FilePath_Upgrade->setText(m_upgradeFilePath);
            }else if(m_upgradeFileType == UPGRADE_FILE_TYPE_XML){
                QTreeWidgetItem *paraItem = GTUtils::findItem("Paratemer", m_saveTreeWidget, 0);
                addParameterToParaItem(&m_increaseItem, paraItem);
                QTreeWidgetItem *upDateInfoItem = GTUtils::findItem("Update Date", m_saveTreeWidget, 0);
                m_insertNum = m_increaseItem.childCount();
                addUpdateInfo(upDateInfoItem);

                QFile file(m_upgradeFilePath);
                file.remove();
                QtTreeManager::writeTreeWidgetToXmlFile(m_upgradeFilePath, m_saveTreeWidget);
            }
            QMessageBox::information(this, tr("Title"), tr("Increase new Parameter successful!"), QMessageBox::Ok);
        }else{
            QMessageBox::information(this, tr("Title!"), tr("No new parameters added!"), QMessageBox::Ok);
        }

    }else{
        QString structOrFileName = ui->lineEdit_StrName->text();
        if(structOrFileName.compare("") == 0){
            structOrFileName = "Parameter";
        }
        if(ui->checkBox_HeadFile->isChecked()){

            QString ExprotFilePath = QFileDialog::getSaveFileName(this, tr("Save export file!"), m_savePath + "/" \
                                                                   + structOrFileName + ".h", tr("Head File(*.h)"));
            if(ExprotFilePath.compare("") == 0){
                QMessageBox::warning(this, tr("Warning!"), tr("No save path confirmed!"), QMessageBox::Ok);
                return ;
            }
            QFile file(ExprotFilePath);
            if(file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
            {
                QTextStream out(&file);
                out << "#ifndef " << structOrFileName.toUpper() << "_H" << endl;
                out << "#define " << structOrFileName.toUpper() << "_H" << endl;
                out << endl;
                out << "typedef enum TypeEnum" << endl;
                out << "{" << "\n";
                out << "\t" << "int16," << endl;
                out << "\t" << "Uint16," << endl;
                out << "\t" << "int32," << endl;
                out << "\t" << "Uint32," << endl;
                out << "}" << "TypeEnumTyp;" << endl;
                out << endl;
                out << "typedef struct " << structOrFileName << "Struct" << endl;
                out << "{" << endl;
                out << "\t" << "int memberSerialNum;" << endl;
                out << "\t" << "int objectWord;" << endl;
                out << "\t" << "char memberName[60];" << endl;
                out << "\t" << "int value;" << endl;
                out << "\t" << "TypeEnumTyp memberType;" << endl;
                out << "\t" << "int addressOffset;" << endl;
                out << "}" << structOrFileName << "StructTyp;" << endl;
                out << endl;
                out << structOrFileName << "StructTyp " << structOrFileName << "[] = {" << "//Struct start" << endl;

                out << "//" << "Create struct at " \
                    << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") \
                    << "\t XmlVersion: " << m_xmlVersion << endl;
                m_memberSerialNum = 0;
                printStructMemberToText(m_treeWidgetItem, out);

                out << "}" << "//Struct end" << endl;
                out << "#endif" << endl;
                out << endl;

                file.close();
                ui->lineEdit_FilePath_Upgrade->clear();
                m_upgradeFilePrepare = false;
                setEnabledExprotPushBtn();
            }
            qDebug() << "\n m_oldParaFileAddr count:\t" << m_oldParaFileAddr.count();
        }else{
            QString ExprotFilePath = QFileDialog::getSaveFileName(this, tr("Save export file!"), m_savePath + "/" \
                                                                   + structOrFileName + ".xml", tr("Xml File(*.xml)"));
            if(ExprotFilePath.compare("") == 0){
                QMessageBox::warning(this, tr("Warning!"), tr("No save path confirmed!"), QMessageBox::Ok);
                return ;
            }
            m_saveTreeWidget->clear();
            QStringList list;
            list<<tr("SerialNum")<<tr("ObjectWord")<<tr("Name")<<tr("Value")<<tr("Type")<<tr("AddressOffset");
            m_saveTreeWidget->setHeaderLabels(list);
            m_saveTreeWidget->setColumnCount(6);

            QTreeWidgetItem *upDateInfoItem = new QTreeWidgetItem;
            upDateInfoItem->setText(0, "Update Date");
            upDateInfoItem->setText(1, "Version");
            upDateInfoItem->setText(2, "InsertNum");
            upDateInfoItem->setText(3, "Sum");
            m_saveTreeWidget->addTopLevelItem(upDateInfoItem);

            QTreeWidgetItem *paraItem = new QTreeWidgetItem;
            paraItem->setText(0, "Paratemer");
            m_saveTreeWidget->addTopLevelItem(paraItem);
            m_memberSerialNum = 0;
            addParameterToParaItem(m_treeWidgetItem, paraItem);

            m_insertNum = m_memberSerialNum;
            addUpdateInfo(upDateInfoItem);

            QtTreeManager::writeTreeWidgetToXmlFile(ExprotFilePath, m_saveTreeWidget);
        }
    }


}

void ParaFileConversion::onCheckBoxUpgradeToggled(bool checked)
{
    if(checked){
        ui->stackedWidget->setCurrentIndex(1);
    }else{
        ui->stackedWidget->setCurrentIndex(0);
    }
    setEnabledExprotPushBtn();
}

void ParaFileConversion::onCheckBoxOutputHeadFileToggled(bool checked)
{
    if(checked){
//        ui->lineEdit_StrName->setEnabled(true);
    }else{
//        ui->lineEdit_StrName->setEnabled(false);
    }
    setEnabledExprotPushBtn();
}

void ParaFileConversion::onPushBtnOpenFileUpgradeClicked()
{
    QString upgradeFilePath = QFileDialog::getOpenFileName(this, tr("Select the parameter file to upgrade!"), m_savePath, tr("parameter file(*.h *.xml)"));
    if(upgradeFilePath.compare("") == 0){
        QMessageBox::warning(this, tr("Warning!"), tr("No file selected!"), QMessageBox::Ok);
        return ;
    }
    QDir dir(upgradeFilePath);
    m_upgradeFilePath = dir.absolutePath();
    ui->lineEdit_FilePath_Upgrade->setText(m_upgradeFilePath);
    dir.cdUp();
    m_upgradeFileUpPath  = dir.absolutePath();

    m_oldParaFileAddr.clear();
    QFileInfo fileInfo(upgradeFilePath);
    QString fileSuffix = fileInfo.suffix();
    if(fileSuffix.compare("h") == 0){
        m_upgradeFileType = UPGRADE_FILE_TYPE_HEAD;
        QString fileName = fileInfo.fileName();
        m_upgradeFileNameCom = fileName.left(fileName.length()-5);

        QFile file(m_upgradeFilePath);
        if(file.open(QFile::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            QString line;
            bool isParaHeadFile = false;
            QString str;

            while(!in.atEnd()){
                line = in.readLine();
                if(isParaHeadFile == true){
                    if(line.contains("},")){
                        str = line.split(',').at(ADDRESS_OFFSET_AT_OUTPUT_STRUCT);
                        m_oldParaFileAddr.append(str.left(str.count()-1));
                    }else if(line.contains("Struct end")){
                        break;
                    }
                }else{
                    if(line.contains("Struct start")){
                        isParaHeadFile = true;
                    }
                }
            }
    //        for(int k = 0; k<m_oldParaFileAddr.count(); k++){
    //            qDebug() << tr("\n m_oldParaFileAddr %1:").arg(k) << m_oldParaFileAddr.at(k);
    //        }
            qDebug() << "\n m_oldParaFileAddr count:\t" << m_oldParaFileAddr.count();
            if(isParaHeadFile == false){
                QMessageBox::warning(this, tr("Warning!"), tr("The file is not the specified parameter head file!"), QMessageBox::Ok);
                ui->lineEdit_FilePath_Upgrade->clear();
                m_upgradeFilePrepare = false;
            }else{
                m_memberSerialNum = m_oldParaFileAddr.count();
                m_upgradeFilePrepare = true;
            }
            file.close();
        }
    }else if(fileSuffix.compare("xml") == 0){
        m_upgradeFileType = UPGRADE_FILE_TYPE_XML;
        QString fileName = fileInfo.fileName();
        m_upgradeFileNameCom = fileName.left(fileName.length()-7);

        QTreeWidget *tree = NULL;
        tree = QtTreeManager::createTreeWidgetFromXmlFile(m_upgradeFilePath);
        if(tree==NULL){
            QMessageBox::warning(this, tr("Warning!"), tr("The file could not be opened!"), QMessageBox::Ok);
            return ;
        }
        qDebug() << "tree->topLevelItem(1)->text(0):" << tree->topLevelItem(1)->text(0);

        m_saveTreeWidget->clear();
        QTreeWidgetItem *item = tree->headerItem()->clone();
        m_saveTreeWidget->setHeaderItem(item);
        for(int i = 0; i<tree->topLevelItemCount(); i++){
            item = tree->topLevelItem(i)->clone();
            m_saveTreeWidget->addTopLevelItem(item);
        }
        delete tree;
        m_saveTreeWidget->expandAll();
        qDebug() << "m_saveTreeWidget->topLevelItem(1)->text(0):" << m_saveTreeWidget->topLevelItem(1)->text(0);

        QTreeWidgetItem *paraItem = GTUtils::findItem("Paratemer", m_saveTreeWidget, 0);
        if(paraItem != NULL){
            for(int i = 0; i<paraItem->childCount(); i++){
                QString paraOffset = paraItem->child(i)->text(ADDRESS_OFFSET_AT_OUTPUT_TREE);
                m_oldParaFileAddr.append(paraOffset);
            }
            m_paraSum = m_oldParaFileAddr.count();
            m_memberSerialNum = m_oldParaFileAddr.count();
            m_upgradeFilePrepare = true;
        }else{
            QMessageBox::warning(this, tr("Warning!"), tr("The file is not the specified parameter head file!"), QMessageBox::Ok);
            ui->lineEdit_FilePath_Upgrade->clear();
            m_upgradeFilePrepare = false;
        }
    }
    setEnabledExprotPushBtn();
}

