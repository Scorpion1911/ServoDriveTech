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


#define MEMBER_NAME_IN_TREEWIDGETITEM 0
#define MEMBER_TYPE_IN_TREEWIDGETITEM 2
#define ADDRESS_OFFSET_IN_TREEWIDGETITEM 3
#define ADDRESS_OFFSET_IN_STRUCT 3
#define RETURN_AND_NEWLINE 2
#define XMLVERSION_IN_TREEWIDGET 1

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
    connect(ui->pushBtn_OpenFile_Upgrade, SIGNAL(clicked()), this, SLOT(onPushBtnOpenFileUpgradeClicked()));
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(false);
    ui->pushbtn_ExportFile->setEnabled(false);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_memberType.append("int16");
    m_memberType.append("Uint16");
    m_memberType.append("int32");
    m_memberType.append("Uint32");
    ui->widget_Upgrade->setVisible(false);
    m_savePath = ".";
    m_xmlPrepare = false;
    m_headPrepare = false;
}

//bool ParaFileConversion::findAxisParameter(QTreeWidget *treeWidget)
//{
//    m_axisList.clear();
//    for(int i = 0; i<treeWidget->topLevelItemCount(); i++){
//        QString str = treeWidget->topLevelItem(i)->text(0);
//        if(str.contains("SevDrvRoot")){
//           m_axisList.append(str);
//        }
//    }
//    if(m_axisList.count()==0){
//        return false;
//    }
//    return true;
//}

void ParaFileConversion::printStructMemberToText(QTreeWidgetItem *treeWidgetItem, QTextStream &out)
{
    for(int i = 0; i<treeWidgetItem->childCount(); i++){
        if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_IN_TREEWIDGETITEM).compare("-1") == 0){
        }else{
            out << "\t" << "{" << QString::number(m_memberSerialNum) << "," \
                                << treeWidgetItem->child(i)->text(1) << "," ;
            int k;
            for(k = 0; k<m_memberType.count(); k++){
                if(treeWidgetItem->child(i)->text(MEMBER_TYPE_IN_TREEWIDGETITEM).compare(m_memberType.at(k)) == 0)
                    break;
            }
            if(k<m_memberType.count()){
                out << treeWidgetItem->child(i)->text(MEMBER_TYPE_IN_TREEWIDGETITEM) << "," ;
            }
            else{
                out << "Uint16" << "," ;
            }
            out << treeWidgetItem->child(i)->text(ADDRESS_OFFSET_IN_TREEWIDGETITEM) << "," \
                << "\"" << treeWidgetItem->child(i)->text(MEMBER_NAME_IN_TREEWIDGETITEM) << "\"" << "}," << endl;

            m_memberSerialNum++;
        }
        if(treeWidgetItem->child(i)->childCount() !=0 ){
            printStructMemberToText(treeWidgetItem->child(i), out);
        }
    }
}

void ParaFileConversion::setEnabledExprotPushBtn()
{
    if(ui->checkBox_Upgrade->isChecked()){
        if(m_xmlPrepare && m_headPrepare){
            ui->pushbtn_ExportFile->setEnabled(true);
        }else{
            ui->pushbtn_ExportFile->setEnabled(false);
        }
    }else{
        if(m_xmlPrepare){
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
        if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_IN_TREEWIDGETITEM).compare("-1") != 0){
            for(k = 0; k<m_oldParaFileAddr.count(); k++){
               if(treeWidgetItem->child(i)->text(ADDRESS_OFFSET_IN_TREEWIDGETITEM).compare(m_oldParaFileAddr.at(k)) == 0){
                 break;
               }
            }
            if(k == m_oldParaFileAddr.count()){
                QTreeWidgetItem *item = new QTreeWidgetItem;
                for(int j = 0; j<treeWidgetItem->child(i)->columnCount(); j++){
                    item->setText(j, treeWidgetItem->child(i)->text(j));
                }
                m_increaseItem.addChild(item);
                m_oldParaFileAddr.append(item->text(ADDRESS_OFFSET_IN_TREEWIDGETITEM));
            }
        }
        if(treeWidgetItem->child(i)->childCount() !=0 ){
            findOutIncreaseParameter(treeWidgetItem->child(i));
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

        m_xmlPrepare = true;
        m_treeWidgetItem = GTUtils::findItem("SevDrvRoot0", ui->treeWidget, 0);
        QTreeWidgetItem *xmlVersionItem = GTUtils::findItem("XmlVersion", ui->treeWidget, 0);
        if(xmlVersionItem != NULL){
            m_xmlVersion = xmlVersionItem->text(XMLVERSION_IN_TREEWIDGET);
        }else{
            m_xmlVersion = "XXX";
        }

//        ui->pushbtn_ExportFile->setEnabled(true);
//        for(int i = 0; i<m_axisList.count(); i++){
//            ui->comboBox->addItem(m_axisList.at(i));
//        }
//        ui->comboBox->setCurrentIndex(0);
    }
    else{
        m_xmlPrepare = false;
        ui->lineEdit_FilePath->clear();
//        ui->pushbtn_ExportFile->setEnabled(false);
        QMessageBox::warning(this, tr("Warning!"), tr("This file is not the specified parameter file!\nPlease re-select!"), QMessageBox::Ok);
        return ;
    }
    setEnabledExprotPushBtn();
}

//void ParaFileConversion::onComboBoxIndexChanged(int index)
//{
//    for(int i = 0; i<ui->treeWidget->topLevelItemCount(); i++){
//        if(ui->treeWidget->topLevelItem(i)->text(0).compare(m_axisList.at(index)) == 0){
//            ui->treeWidget->topLevelItem(i)->setExpanded(true);
//            m_treeWidgetItem = ui->treeWidget->topLevelItem(i);
//        }
//        else{
//            ui->treeWidget->topLevelItem(i)->setExpanded(false);
//        }
//    }
//}

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
            QFile file(m_headFilePath);
            int seekCharNum = 0;
    //        int rowNum = 0;
            if(file.open(QFile::ReadWrite | QIODevice::Text)){
                QTextStream inOrOut(&file);
                while(!inOrOut.atEnd()){
                    str = inOrOut.readLine();
                    if(str.contains("Struct end")){
                        break;
                    }
    //                qDebug() << tr("Row%1 char length: %2").arg(++rowNum).arg(str.length() + RETURN_AND_NEWLINE);
                    seekCharNum += (str.length() + RETURN_AND_NEWLINE);
    //                if(str.contains("Struct start")){
    //                    isStructStart = true;
    //                    for(int k = 0; k<m_oldParaFileAddr.count(); k++){
    //                        str = inOrOut.readLine();
    //                        qDebug() << tr("Row%1 char length: %2").arg(++rowNum).arg(str.length() + RETURN_AND_NEWLINE);
    //                        seekCharNum += (str.length() + RETURN_AND_NEWLINE);
    //                    }
    //                    break;
    //                }
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
            QString newHeadFile = m_headFileUpPath + "/" + m_headFileNameCom + m_xmlVersion + ".h";
            QFile::rename(m_headFilePath, newHeadFile);
            m_headFilePath = newHeadFile;
            ui->lineEdit_FilePath_Upgrade->setText(m_headFilePath);

            QMessageBox::information(this, tr("Title"), tr("Increase new Parameter successful!"), QMessageBox::Ok);
//            ui->lineEdit_FilePath_Upgrade->clear();
//            m_headPrepare = false;
//            setEnabledExprotPushBtn();
        }else{
            QMessageBox::information(this, tr("Title!"), tr("No new parameters added!"), QMessageBox::Ok);
        }

    }else{
        QString structName = ui->lineEdit_StrName->text();
        if(structName.compare("") == 0){
            structName = "Parameter";
        }
        QString ExprotFilePath = QFileDialog::getSaveFileName(this, tr("Save export file!"), m_savePath + "/" \
                                                              + structName + m_xmlVersion + ".h", tr("Head File(*.h)"));
        if(ExprotFilePath.compare("") == 0){
            QMessageBox::warning(this, tr("Warning!"), tr("No save path confirmed!"), QMessageBox::Ok);
            return ;
        }
        QFile file(ExprotFilePath);
        if(file.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "#ifndef " << structName.toUpper() << "_H" << endl;
            out << "#define " << structName.toUpper() << "_H" << endl;
            out << endl;
            out << "typedef enum TypeEnum" << endl;
            out << "{" << "\n";
            out << "\t" << "int16," << endl;
            out << "\t" << "Uint16," << endl;
            out << "\t" << "int32," << endl;
            out << "\t" << "Uint32," << endl;
            out << "}" << "TypeEnumTyp;" << endl;
            out << endl;
            out << "typedef struct " << structName << "Struct" << endl;
            out << "{" << endl;
            out << "\t" << "int memberSerialNum;" << endl;
            out << "\t" << "int value;" << endl;
            out << "\t" << "TypeEnumTyp memberType;" << endl;
            out << "\t" << "int addressOffset;" << endl;
            out << "\t" << "char memberName[60];" << endl;
            out << "}" << structName << "StructTyp;" << endl;
            out << endl;
            out << structName << "StructTyp " << structName << "[] = {" << "//Struct start" << endl;

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
            m_headPrepare = false;
            setEnabledExprotPushBtn();
        }
    }
    qDebug() << "\n m_oldParaFileAddr count:\t" << m_oldParaFileAddr.count();

}

void ParaFileConversion::onCheckBoxUpgradeToggled(bool checked)
{
    if(checked){
        ui->widget_Upgrade->setVisible(true);
        ui->lineEdit_StrName->setEnabled(false);
    }
    else{
        ui->widget_Upgrade->setVisible(false);
        ui->lineEdit_StrName->setEnabled(true);
    }
    setEnabledExprotPushBtn();
}

void ParaFileConversion::onPushBtnOpenFileUpgradeClicked()
{
    QString headFilePath = QFileDialog::getOpenFileName(this, tr("Select parameter Head file!"), m_savePath, tr("Head file(*.h)"));
    if(headFilePath.compare("") == 0){
        QMessageBox::warning(this, tr("Warning!"), tr("No file selected!"), QMessageBox::Ok);
        return ;
    }

    QFileInfo fileInfo(headFilePath);
    QString fileName = fileInfo.fileName();
    m_headFileNameCom = fileName.left(fileName.length()-5);

    QDir dir(headFilePath);
    m_headFilePath = dir.absolutePath();
    ui->lineEdit_FilePath_Upgrade->setText(m_headFilePath);
    dir.cdUp();
    m_headFileUpPath  = dir.absolutePath();

    QFile file(m_headFilePath);
    if(file.open(QFile::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        QString line;
        bool isParaHeadFile = false;
        QString str;
        m_oldParaFileAddr.clear();

        while(!in.atEnd()){
            line = in.readLine();

            if(isParaHeadFile == true){
                if(line.contains("},")){
                    str = line.split(',').at(ADDRESS_OFFSET_IN_STRUCT);
                    m_oldParaFileAddr.append(str);
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
            m_headPrepare = false;
        }else{
            m_headPrepare = true;
            m_memberSerialNum = m_oldParaFileAddr.count();
        }
        file.close();
    }
    setEnabledExprotPushBtn();
}

