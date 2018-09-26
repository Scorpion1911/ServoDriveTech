#include "motordbUi.h"
#include "ui_motordbUi.h"
#include "motordbmanager.h"
//#include "doublespinboxdelegate.h"
#include "gtutils.h"

#include <QList>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>

#define PARA_NAME_INDEX 0
#define PARA_VALUE_INDEX 1
#define PARA_UNIT_INDEX 2

#define USER_INDEX 5

MotorDBUi::MotorDBUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MotorDBUi)
{
    ui->setupUi(this);
    initMap();
    initUi();
    createConnections();
}

MotorDBUi::~MotorDBUi()
{
    qDebug()<<"motor db ui deleted";
    delete m_dbManager;
    delete ui;
}

bool MotorDBUi::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        GTUtils::delayms(100);
        if(obj == ui->table_motor_para)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            qDebug()<<keyEvent->key();
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                if (m_isEditing) {
                    if (m_editedItem != NULL) {
                        QString text = m_editedItem->text();
                        bool isOk;
                        double value = text.toDouble(&isOk);
                        if (isOk) {
                            m_isEditValid = true;
                        } else {
                            m_isEditValid = false;
                        }
                        onItemEditingFinished();
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MotorDBUi::initUi()
{
    QString dbPath = GTUtils::databasePath() + "Motor/";
    m_dbManager = new MotorDBManager(dbPath, "", "");
    m_companyIdList = m_dbManager->getCompanyIdList();

    m_editedItem = NULL;
    m_isEditing = false;
    m_isEditValid = false;

    for (int i = 0; i < m_unitMap.keys().count(); i++) {
        ui->table_motor_para->insertRow(i);
        QTableWidgetItem* nameItem = new QTableWidgetItem(m_map.value(m_paraNameList.at(i)));
        QTableWidgetItem* unitItem = new QTableWidgetItem(m_unitMap.value(m_paraNameList.at(i)));
        ui->table_motor_para->setItem(i, PARA_NAME_INDEX, nameItem);
        ui->table_motor_para->setItem(i, PARA_UNIT_INDEX, unitItem);
        nameItem->setFlags(Qt::NoItemFlags);
        unitItem->setFlags(Qt::NoItemFlags);
    }
    for (int i = 0; i < m_companyIdList.count(); i++) {
        QString companyName = m_dbManager->getCompanyName(m_companyIdList.at(i));
        ui->listWidget_motor_company->addItem(companyName);
    }
    ui->listWidget_motor_company->setCurrentRow(0);
    onCompanyRowChanged(0);
    ui->table_motor_para->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->widget_newMotor->setVisible(false);
    ui->table_motor_para->installEventFilter(this);
    //DoubleSpinBoxDelegate delegate;
    //ui->table_motor_para->setItemDelegateForColumn(PARA_VALUE_INDEX, &delegate);
}

void MotorDBUi::initMap()
{
    m_map.insert("Imax", tr("Imax"));
    m_map.insert("Irat", tr("Irat"));
    m_map.insert("Sct", tr("Sct"));
    m_map.insert("Srat", tr("Srat"));
    m_map.insert("Nos", tr("Nos"));
    m_map.insert("Rm", tr("Rm"));
    m_map.insert("Ldm", tr("Ldm"));
    m_map.insert("Lqm", tr("Lqm"));
    m_map.insert("Jm", tr("Jm"));
    m_map.insert("Jrat", tr("Jrat"));
    m_map.insert("Fm", tr("Fm"));
    m_map.insert("PPN", tr("PPN"));
    m_map.insert("Tqr", tr("Tqr"));
    m_map.insert("PHIm", tr("Moment Coefficient"));
    m_map.insert("Vmax", tr("Vmax"));

    m_unitMap.insert("Imax", "A(peak)");
    m_unitMap.insert("Irat", "A(rms)");
    m_unitMap.insert("Sct", "rpm");
    m_unitMap.insert("Srat", "%");
    m_unitMap.insert("Nos", "rpm");
    m_unitMap.insert("Rm", "Ohm");
    m_unitMap.insert("Ldm", "mH");
    m_unitMap.insert("Lqm", "mH");
    m_unitMap.insert("Jm", "10^-6.kg.m^2");
    m_unitMap.insert("Jrat", "%");
    m_unitMap.insert("Fm", "10^-4.N.m/(rad/s)");
    m_unitMap.insert("PPN", "pairs");
    m_unitMap.insert("Tqr", "N.m");
    m_unitMap.insert("PHIm", "N.m/Arms");
    m_unitMap.insert("Vmax", "V");

    m_paraNameList.append("Imax");
    m_paraNameList.append("Irat");
    m_paraNameList.append("Sct");
    m_paraNameList.append("Srat");
    m_paraNameList.append("Nos");
    m_paraNameList.append("Rm");
    m_paraNameList.append("Ldm");
    m_paraNameList.append("Lqm");
    m_paraNameList.append("Jm");
    m_paraNameList.append("Jrat");
    m_paraNameList.append("Fm");
    m_paraNameList.append("PPN");
    m_paraNameList.append("Tqr");
    m_paraNameList.append("PHIm");
    m_paraNameList.append("Vmax");

}

void MotorDBUi::createConnections()
{
    connect(ui->btn_motor_return, SIGNAL(clicked()), this, SIGNAL(returnClicked()));
    connect(ui->listWidget_motor_company, SIGNAL(currentRowChanged(int)), this, SLOT(onCompanyRowChanged(int)));
    //connect(ui->listWidget_motor_motor, SIGNAL(currentRowChanged(int)), this, SLOT(onMotorRowChanged(int)));
    connect(ui->btn_motor_new, SIGNAL(clicked()), this, SLOT(onActionNewBtnClicked()));
    connect(ui->btn_motor_remove, SIGNAL(clicked()), this, SLOT(onActionRemoveBtnClicked()));
    connect(ui->btn_motor_save, SIGNAL(clicked()), this, SLOT(onActionSaveBtnClicked()));
    connect(ui->btn_motor_add, SIGNAL(clicked()), this, SLOT(onActionAddBtnClicked()));
    connect(ui->btn_motor_install, SIGNAL(clicked()), this, SLOT(onActionInstallBtnClicked()));
    connect(ui->btn_motor_ok, SIGNAL(clicked()), this, SLOT(onActionOkBtnClicked()));
    connect(ui->btn_motor_cancel, SIGNAL(clicked()), this, SLOT(onActionCancelBtnClicked()));
    connect(ui->table_motor_para, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(onItemDoubleClicked(QTableWidgetItem*)));
    connect(ui->table_motor_para, SIGNAL(itemSelectionChanged()), this, SLOT(onItemEditingFinished()));
}

void MotorDBUi::onCompanyRowChanged(int index)
{
    onItemEditingFinished();
    int companyId = m_companyIdList.at(index);
    if (companyId == USER_INDEX) {
        ui->btn_motor_new->setEnabled(true);
        ui->btn_motor_remove->setEnabled(true);
        ui->btn_motor_save->setEnabled(true);
    } else {
        ui->btn_motor_new->setEnabled(false);
        ui->btn_motor_remove->setEnabled(false);
        ui->btn_motor_save->setEnabled(false);
    }
    m_motorIdList.clear();
    disconnect(ui->listWidget_motor_motor, SIGNAL(currentRowChanged(int)), this, SLOT(onMotorRowChanged(int)));
    ui->listWidget_motor_motor->clear();
    m_motorIdList = m_dbManager->getMotorIdList(companyId);
    for (int i = 0; i < m_motorIdList.count(); i++) {
        QString motorName = m_dbManager->getMotorName(m_motorIdList.at(i));
        ui->listWidget_motor_motor->addItem(motorName);
    }
    connect(ui->listWidget_motor_motor, SIGNAL(currentRowChanged(int)), this, SLOT(onMotorRowChanged(int)));
    ui->listWidget_motor_motor->setCurrentRow(0);
    onMotorRowChanged(0);
}

void MotorDBUi::onMotorRowChanged(int index)
{
    onItemEditingFinished();
    int motorId = m_motorIdList.at(index);
    m_motorParaList.clear();
    for (int i = 0; i < m_map.keys().count(); i++) {
        //qDebug()<<"i"<<i;
        double value = m_dbManager->getMotorPara(motorId, m_paraNameList.at(i));
        QTableWidgetItem *valueItem = new QTableWidgetItem(QString::number(value));
        m_motorParaList.append(QString::number(value));
        //qDebug()<<"value"<<m_motorParaList.at(i);
        QTableWidgetItem *item = ui->table_motor_para->item(i, PARA_VALUE_INDEX);
        if (item != NULL) {
            delete item;
        }
        ui->table_motor_para->setItem(i, PARA_VALUE_INDEX, valueItem);
    }
}

void MotorDBUi::onActionNewBtnClicked()
{
    ui->lineEdit_motor_newName->clear();
    ui->widget_motor->setVisible(false);
    ui->widget_newMotor->setVisible(true);
    m_motorParaList.clear();
    for (int i = 0; i < ui->table_motor_para->rowCount(); i++) {
        QTableWidgetItem *valueItem = new QTableWidgetItem("0");
        m_motorParaList.append("0");
        QTableWidgetItem *item = ui->table_motor_para->item(i, PARA_VALUE_INDEX);
        if (item != NULL) {
            delete item;
        }
        ui->table_motor_para->setItem(i, PARA_VALUE_INDEX, valueItem);
    }
}

void MotorDBUi::onActionRemoveBtnClicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Motor info"), tr("Are you sure to remove the motor?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (!(rb == QMessageBox::Yes)) {
        return;
    }
    int motorId = m_motorIdList.at(ui->listWidget_motor_motor->currentRow());
    m_dbManager->removeMotor(motorId);
    onCompanyRowChanged(ui->listWidget_motor_company->currentRow());
    //ui->listWidget_motor_motor->removeItemWidget(ui->listWidget_motor_motor->currentItem());
}

void MotorDBUi::onActionSaveBtnClicked()
{
    QStringList motorInfoList;
    int motorId = m_motorIdList.at(ui->listWidget_motor_motor->currentRow());
    motorInfoList<<QString::number(motorId);
    motorInfoList<<ui->listWidget_motor_motor->currentItem()->text();
    int companyId = m_companyIdList.at(ui->listWidget_motor_company->currentRow());
    motorInfoList<<QString::number(companyId);
    for (int i = 0; i < m_motorParaList.count(); i++) {
        QString motorPara = m_motorParaList.at(i);
        motorInfoList<<motorPara;
    }
//    for (int i = 0; i < ui->table_motor_para->rowCount(); i++) {
//        QString motorPara = ui->table_motor_para->item(i, PARA_VALUE_INDEX)->text();
//        motorInfoList<<motorPara;
//    }
    m_dbManager->changeMotor(motorInfoList);
    onMotorRowChanged(ui->listWidget_motor_motor->currentRow());
}

void MotorDBUi::onActionAddBtnClicked()
{
    ui->lineEdit_motor_newName->clear();
    ui->widget_motor->setVisible(false);
    ui->widget_newMotor->setVisible(true);
}

void MotorDBUi::onActionInstallBtnClicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Motor info"), tr("Are you sure to install the motor?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (!(rb == QMessageBox::Yes)) {
        return;
    }
    emit onMotorInstall(m_motorParaList);
}

void MotorDBUi::onActionOkBtnClicked()
{
    QString motorName = ui->lineEdit_motor_newName->text();
    if (motorName.compare("") == 0) {
        QMessageBox::information(0, tr("Motor Info"), tr("Motor name cannot be empty!"));
        return;
    }
    for (int i = 0; i < ui->listWidget_motor_motor->count(); i++) {
        QString name = ui->listWidget_motor_motor->item(i)->text();
        if (name.compare(motorName) == 0) {
            QMessageBox::information(0, tr("Motor Info"), tr("This motor has already existed. Please select another name."));
            return;
        }
    }    

    QStringList motorInfoList;
    int motorSeq = m_dbManager->getMotorSeq() + 1;
    motorInfoList<<QString::number(motorSeq);
    motorInfoList<<motorName;
    //int companyId = m_companyIdList.at(ui->listWidget_motor_company->currentRow()) + 1;
    int companyId = USER_INDEX;
    motorInfoList<<QString::number(companyId);
    for (int i = 0; i < m_motorParaList.count(); i++) {
        QString motorPara = m_motorParaList.at(i);
        motorInfoList<<motorPara;
    }
//    for (int i = 0; i < ui->table_motor_para->rowCount(); i++) {
//        QString motorPara = ui->table_motor_para->item(i, PARA_VALUE_INDEX)->text();
//        motorInfoList<<motorPara;
//    }
    m_dbManager->addMotor(motorInfoList);

    //onCompanyRowChanged(ui->listWidget_motor_company->currentRow());
    if (ui->listWidget_motor_company->currentRow() == USER_INDEX - 1) {
        onCompanyRowChanged(ui->listWidget_motor_company->currentRow());
        ui->listWidget_motor_motor->setCurrentRow(ui->listWidget_motor_motor->count() - 1);
        onMotorRowChanged(ui->listWidget_motor_motor->count() - 1);
    }

//    ui->listWidget_motor_motor->addItem(ui->lineEdit_motor_newName->text());
//    ui->listWidget_motor_motor->setCurrentRow(ui->listWidget_motor_motor->count() - 1);

//    m_motorIdList.append(motorSeq);


//    QMessageBox::StandardButton rb = QMessageBox::question(0, tr("Motor info"), tr("Are you sure to add the motor?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
//    if (!(rb == QMessageBox::Yes)) {
//        return;
//    }

    ui->lineEdit_motor_newName->clear();
    ui->widget_motor->setVisible(true);
    ui->widget_newMotor->setVisible(false);
}

void MotorDBUi::onActionCancelBtnClicked()
{
    ui->lineEdit_motor_newName->clear();
    ui->widget_motor->setVisible(true);
    ui->widget_newMotor->setVisible(false);
    onMotorRowChanged(ui->listWidget_motor_motor->currentRow());
}

void MotorDBUi::onItemDoubleClicked(QTableWidgetItem *item)
{
    if (item == NULL) {
        return;
    }
    if (item->column() == PARA_VALUE_INDEX) {
        m_originText = item->text();
        m_isEditing = true;
        m_editedItem = item;
        m_isEditValid = false;
    }
}

void MotorDBUi::onItemEditingFinished()
{
    if (m_isEditing) {
        if (!m_isEditValid) {
            m_editedItem->setText(m_originText);
        } else {
            int row = m_editedItem->row();
            m_motorParaList.replace(row, m_editedItem->text());
            m_editedItem->setTextColor(Qt::black);
            m_editedItem->setBackgroundColor(Qt::yellow);
        }
        m_isEditing = false;
        m_originText = "";
        m_editedItem = NULL;
    }
}
