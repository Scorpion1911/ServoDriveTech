#include "selfbuildermainwindow.h"
#include "ui_selfbuildermainwindow.h"
#include "selfbuilder.h"
#include "gtutils.h"
#include "rnnet.h"
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

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onOpenClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onAssignClicked()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(onSaveClicked()));

    qint16 a = 20000;
    int b = 20000;
    int c = a + b;
    qDebug()<<"c = "<<c;
}

SelfBuilderMainWindow::~SelfBuilderMainWindow()
{
    delete ui;
}

void SelfBuilderMainWindow::onOpenClicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Files(*.ui)"));
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
    QTreeWidgetItemIterator it(m_tree);
    int addr = 0;
    while (*it) {
        if ((*it)->text(6).compare("-1") != 0) {
            (*it)->setText(6, QString("0x%1").arg(addr, 4, 16, QLatin1Char('0')));
            QString type = (*it)->text(5);
            if (type.compare("Uint8") == 0 || type.compare("int8") == 0 || type.compare("Case") == 0) {
                addr = addr + 1;
            } else if (type.compare("Uint16") == 0 || type.compare("int16") == 0) {
                addr = addr + 2;
            } else if (type.compare("Uint32") == 0 || type.compare("int32") == 0) {
                addr = addr + 4;
            }
        }
        ++it;
    }
}

void SelfBuilderMainWindow::onSaveClicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("Files(*.ui)"));
    if (path.length() == 0) {
        QMessageBox::information(NULL, tr("Path"), tr("You did not select any files."));
    } else {
        QMessageBox::information(NULL, tr("Path"), tr("You selected ") + path);
        QtTreeManager::writeTreeWidgetToXmlFile(path, m_tree);
    }
}
