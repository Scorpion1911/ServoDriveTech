#include "selfbuildermainwindow.h"
#include "ui_selfbuildermainwindow.h"
#include "selfbuilder.h"
#include "gtutils.h"
#include "rnnet.h"
#include "dbmanager.h"
#include "qttreemanager.h"

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>


SelfBuilderMainWindow::SelfBuilderMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SelfBuilderMainWindow)
{
    ui->setupUi(this);
//    QString path = GTUtils::commonPath();
//    qDebug()<<path;
//    BuilderParameters *para = new BuilderParameters(33333, 55555, "V131");
//    BuilderParameters para(21000541, 21000543, "V130");
//    ComDriver::ICom *tempCom = new ComDriver::RnNet();
//    int ret = tempCom->open(0, 0);
//    qDebug()<<"ret = "<<ret;
//    SelfBuilder *builder = new SelfBuilder(tempCom);
//    SelfBuilder::RtnSelf rtn =  builder->buildFromEprom(0, 0, &para);
//    qDebug()<<"rtn = "<<rtn;

//    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onOpenClicked()));
//    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onAssignClicked()));
//    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(onSaveClicked()));

//    QTreeWidget* newTree = QtTreeManager::createTreeWidgetFromXmlFile("C:/Users/googol/Desktop/3/PrmPrtyTree.xml");
//    QTreeWidget* oldTree = QtTreeManager::createTreeWidgetFromXmlFile("E:/Jiang/ServoDriveTech/build/debug/sysconfig/SD6x/SD61_PLUS/V139/PrmPrtyTree.xml");
//    QList<QTreeWidgetItem*> list0;
//    QList<QTreeWidgetItem*> list1;

//    for (int i = 5; i < 10; i++) {
//        list0.append(newTree->topLevelItem(0)->child(0)->child(2)->child(i)->clone());
//        list1.append(newTree->topLevelItem(1)->child(0)->child(2)->child(i)->clone());
//    }
//    list0.at(0)->child(0)->setText(1, "240");
//    list1.at(0)->child(0)->setText(1, "240");
//    qDebug()<<"count"<<oldTree->topLevelItemCount() - 1;
//    for (int i = 0; i < oldTree->topLevelItemCount() - 1; i++) {
//        list0.clear();
//        list1.clear();
//        for (int j = 5; j < 10; j++) {
//            list0.append(newTree->topLevelItem(0)->child(0)->child(2)->child(j)->clone());
//            list1.append(newTree->topLevelItem(1)->child(0)->child(2)->child(j)->clone());
//        }
//        list0.at(0)->child(0)->setText(1, "240");
//        list1.at(0)->child(0)->setText(1, "240");
//        if (i % 2 == 0) {
//            qDebug()<<"i = "<<i;
//            oldTree->topLevelItem(i + 1)->child(0)->child(2)->addChildren(list0);
//        } else {
//            qDebug()<<"i = "<<i;
//            oldTree->topLevelItem(i + 1)->child(0)->child(2)->addChildren(list1);
//        }
//    }
//    QtTreeManager::writeTreeWidgetToXmlFile("E:/Jiang/ServoDriveTech/build/debug/sysconfig/SD6x/SD61_PLUS/V139/FlashPrm_AllAxis.xml", oldTree);
//    delete oldTree;
//    delete newTree;


//    for (int i = 0; i < oldTree->topLevelItemCount(); i++) {
//        list0.clear();
//        for (int j = 5; j < 10; j++) {
//            list0.append(newTree->topLevelItem(0)->child(0)->child(2)->child(j)->clone());
//        }
//        oldTree->topLevelItem(i)->child(0)->child(2)->addChildren(list0);
//    }
//    QtTreeManager::writeTreeWidgetToXmlFile("E:/Jiang/ServoDriveTech/build/debug/sysconfig/SD6x/SD61_PLUS/V139/PrmPrtyTree.xml", oldTree);
//    delete oldTree;
//    delete newTree;
//    qDebug()<<"finish";

    quint32 a = 12055488;
    quint32 b = 16777216;
    qint64 dd = a * 360;
    qDebug()<<"dd = "<<dd;
    double c = dd / b;
    qDebug()<<"c = "<<c;
}

SelfBuilderMainWindow::~SelfBuilderMainWindow()
{
    delete ui;
}

void SelfBuilderMainWindow::onOpenClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Files(*.xml)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        m_tree = QtTreeManager::createTreeWidgetFromXmlFile(path);
        //m_tree->show();
    }
}

void SelfBuilderMainWindow::onAssignClicked()
{
//    QTreeWidgetItemIterator it(m_tree);
//    int addr = 0;
//    while (*it) {
//        if ((*it)->text(6).compare("-1") != 0) {
//            (*it)->setText(6, QString("0x%1").arg(addr, 4, 16, QLatin1Char('0')));
//            QString type = (*it)->text(5);
//            if (type.compare("Uint8") == 0 || type.compare("int8") == 0 || type.compare("Case") == 0) {
//                addr = addr + 1;
//            } else if (type.compare("Uint16") == 0 || type.compare("int16") == 0) {
//                addr = addr + 2;
//            } else if (type.compare("Uint32") == 0 || type.compare("int32") == 0) {
//                addr = addr + 4;
//            }
//        }
//        ++it;
//    }
    for (int i = 0; i < 5; i++) {
        QTreeWidgetItem *item = m_tree->topLevelItem(0)->clone();
        if (i % 2 == 0) {
            changeAddr(item);
        }
        m_tree->addTopLevelItem(item);
    }
}

void SelfBuilderMainWindow::onSaveClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Files(*.xml)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        QtTreeManager::writeTreeWidgetToXmlFile(path, m_tree);
    }
}

void SelfBuilderMainWindow::changeAddr(QTreeWidgetItem *item)
{
    if (item != NULL) {
        QString addr = item->text(3);
        if (addr.compare("-1") != 0 && addr.compare("") != 0) {
            int add = addr.toInt();
            add = add + 32768;
            item->setText(3, QString::number(add));
        }
        for (int i = 0; i < item->childCount(); i++) {
            changeAddr(item->child(i));
        }
    }
}
