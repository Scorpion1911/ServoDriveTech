#include "appfillmaxrangetest.h"
#include "ui_appfillmaxrangetest.h"
#include "qttreemanager.h"

#include <QFileDialog>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QTreeWidgetItem>

AppFillMaxRangeTest::AppFillMaxRangeTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppFillMaxRangeTest)
{
    ui->setupUi(this);
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(onActionToolBtnClicked()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onActionFillBtnClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onActionSaveBtnClicked()));
}

AppFillMaxRangeTest::~AppFillMaxRangeTest()
{
    delete ui;
}

void AppFillMaxRangeTest::onActionToolBtnClicked()
{
    m_filePath = QFileDialog::getOpenFileName(this, "Select Xml Files", ".", "*.xml");
}

void AppFillMaxRangeTest::onActionFillBtnClicked()
{
    m_tree = QtTreeManager::createTreeWidgetFromXmlFile(m_filePath);
    QTreeWidgetItemIterator it(m_tree);
        while (*it) {
            if ((*it)->childCount() == 0) {
                if ((*it)->text(1) == "int32") {
                    (*it)->setText(2, "2147483647");
                    (*it)->setText(3, "-2147483647");
                } else if ((*it)->text(1) == "Uint32") {
                    (*it)->setText(2, "4294967295");
                    (*it)->setText(3, "0");
                } else if ((*it)->text(1) == "int8") {
                    (*it)->setText(2, "127");
                    (*it)->setText(3, "-127");
                } else if ((*it)->text(1) == "Uint8") {
                    (*it)->setText(2, "255");
                    (*it)->setText(3, "0");
                } else if ((*it)->text(1) == "int16") {
                    (*it)->setText(2, "32767");
                    (*it)->setText(3, "-32767");
                } else {
                    (*it)->setText(2, "65535");
                    (*it)->setText(3, "0");
                }
            }
            ++it;
        }
}

void AppFillMaxRangeTest::onActionSaveBtnClicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Save Xml File", ".", "*.xml");
    QtTreeManager::writeTreeWidgetToXmlFile(path, m_tree);
}
