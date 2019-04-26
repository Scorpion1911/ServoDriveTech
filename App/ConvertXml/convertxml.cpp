#include "convertxml.h"
#include "ui_convertxml.h"
#include "qttreemanager.h"

#include <QFileDialog>
#include <QTreeWidget>

ConvertXml::ConvertXml(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConvertXml)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onConvertBtnClicked()));
    m_openFilePath = ".";
}

ConvertXml::~ConvertXml()
{
    delete ui;
}

void ConvertXml::onConvertBtnClicked()
{
    QStringList xmlFilePaths = QFileDialog::getOpenFileNames(this, tr("Please select three XML type files!"), m_openFilePath, tr("XML File(*.xml)"));
    for (int i = 0; i < xmlFilePaths.length(); i++) {
        QString path = xmlFilePaths.at(i);
        QTreeWidget* tree = QtTreeManager::createTreeWidgetFromXmlFile(path);
        if (tree != NULL) {
            QtTreeManager::writeTreeWidgetToXmlFile(path, tree);
        }
    }
}
