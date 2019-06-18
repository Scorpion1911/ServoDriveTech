#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qttreemanager.h"

#include <QFileDialog>
#include <QTreeWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_filePath = "";
    m_path = "";
    m_tree = NULL;
    connect(ui->btn_open, SIGNAL(clicked()), this, SLOT(onOpenClicked()));
    connect(ui->btn_exe, SIGNAL(clicked()), this, SLOT(onExeClicked()));
    connect(ui->btn_save, SIGNAL(clicked()), this, SLOT(onSaveClicked()));
}

MainWindow::~MainWindow()
{
    delete m_tree;
    delete ui;
}

void MainWindow::onOpenClicked()
{
    m_filePath = QFileDialog::getOpenFileName(this, tr("Open File"), m_path, tr("Files(*.xml)"));
    QFileInfo fileInfo;
    fileInfo.setFile(m_filePath);
    m_path = fileInfo.filePath() + "/";
    if (m_filePath.length() == 0) {
        return;
    } else {
        m_tree = QtTreeManager::createTreeWidgetFromXmlFile(m_filePath);
        //m_tree->show();
        qDebug()<<"open finish";
    }
}

void MainWindow::onExeClicked()
{
    if (m_tree == NULL) {
        return;
    }
    if (m_tree->topLevelItemCount() != 2) {
        return;
    }
    int desCount = ui->spinBox->value();
    int count = 0;
    while (m_tree->topLevelItemCount() - 1 < desCount) {
        QTreeWidgetItem *item = m_tree->topLevelItem(1)->clone();
        if (count % 2 == 0) {
            changeAddr(item);
        }
        m_tree->addTopLevelItem(item);
        count++;
    }
    qDebug()<<"exe finish";
}

void MainWindow::onSaveClicked()
{
    if (m_tree != NULL) {
        QtTreeManager::writeTreeWidgetToXmlFile(m_filePath, m_tree);
    }
    qDebug()<<"save finish";
}

void MainWindow::changeAddr(QTreeWidgetItem *item)
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
