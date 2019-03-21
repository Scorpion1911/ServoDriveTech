#include "motordbmanager.h"

#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>


MotorDBManager::MotorDBManager(const QString &dbPath, const QString &userName, const QString &psw, QObject *parent) : QObject(parent)
{
    m_dbPath = dbPath;
    m_usrName = userName;
    m_psw = psw;
}

MotorDBManager::~MotorDBManager()
{
    if (m_motorDB.isOpen()){
        m_motorDB.close();
    }
}

QList<int> MotorDBManager::getCompanyIdList()
{
    QSqlTableModel model;
    model.setTable("Company");
    model.select();
    QList<int> result;
    for (int i = 0; i < model.rowCount(); i++) {
        QSqlRecord record = model.record(i);
        result.append(record.value("Id").toInt());
    }
    return result;
}

QString MotorDBManager::getCompanyName(int id)
{
    QSqlTableModel model;
    model.setTable("Company");
    QString filter = QString("Id = '%1'").arg(id);
    model.setFilter(filter);
    model.select();
    QSqlRecord record = model.record(0);
    QString result = record.value("CompanyName").toString();
    return result;
}

QList<int> MotorDBManager::getMotorIdList(int id)
{
    QSqlTableModel model;
    model.setTable("Motor");
    QString filter = QString("CompanyId = '%1'").arg(id);
    model.setFilter(filter);
    model.select();
    QList<int> result;
    for (int i = 0; i < model.rowCount(); i++) {
        QSqlRecord record = model.record(i);
        result.append(record.value("Id").toInt());
    }
    return result;
}

QString MotorDBManager::getMotorName(int id)
{
    QSqlTableModel model;
    model.setTable("Motor");
    QString filter = QString("Id = '%1'").arg(id);
    model.setFilter(filter);
    model.select();
    QSqlRecord record = model.record(0);
    QString result = record.value("MotorName").toString();
    return result;
}

double MotorDBManager::getMotorPara(int id, const QString &paraName)
{
    QSqlTableModel model;
    model.setTable("Motor");
    QString filter = QString("Id = '%1'").arg(id);
    model.setFilter(filter);
    model.select();
    QSqlRecord record = model.record(0);
    double result = record.value(paraName).toDouble();
    return result;
}

void MotorDBManager::addMotor(const QStringList &list)
{
    QSqlTableModel model;
    model.setTable("Motor");
    int rowCount = model.rowCount();
    model.insertRow(rowCount);
    for (int i = 0; i < model.columnCount(); i++) {
        model.setData(model.index(rowCount, i), list.at(i));
    }
    model.submitAll();
    int motorSeq = getMotorSeq();
    QString newSeq = list.at(0);
    if (newSeq.toInt() > motorSeq) {
        setMotorSeq(newSeq);
    }
}

void MotorDBManager::removeMotor(int motorId)
{
    QSqlTableModel model;
    model.setTable("Motor");
    QString filter = QString("Id = '%1'").arg(motorId);
    model.setFilter(filter);
    model.select();
    if (model.rowCount() == 1) {
        model.removeRow(0);
        model.submitAll();
        int motorSeq = getMotorSeq();
        qDebug()<<"motorSeq"<<motorSeq;
        qDebug()<<"motorId"<<motorId;
        if (motorId == motorSeq) {
            setMotorSeq(QString::number(motorSeq - 1));
        }
    }
}

void MotorDBManager::changeMotor(const QStringList &list)
{
    QSqlTableModel model;
    model.setTable("Motor");
    QString motorId = list.at(0);
    QString filter = "Id = " + motorId;
    model.setFilter(filter);
    model.select();
    QSqlRecord record = model.record(0);
    for (int i = 0; i < list.count(); i++) {
        record.setValue(i, list.at(i));
    }
    model.setRecord(0, record);
    model.submitAll();
}

int MotorDBManager::getMotorSeq()
{
    QSqlTableModel model;
    model.setTable("sqlite_sequence");
    model.select();
    qDebug()<<"record count"<<model.record().count();
    QSqlRecord record = model.record(1);
    int result = record.value("seq").toInt();
    return result;
}

int MotorDBManager::getCompanySeq()
{
    QSqlTableModel model;
    model.setTable("sqlite_sequence");
    model.select();
    QSqlRecord record = model.record(0);
    int result = record.value("seq").toInt();
    return result;
}

bool MotorDBManager::open()
{
    m_motorDB = QSqlDatabase::addDatabase("QSQLITE");
    m_motorDB.setDatabaseName(m_dbPath + "MotorInfomation.sqlite");
    m_motorDB.setUserName(m_usrName);
    m_motorDB.setPassword(m_psw);
    if (!m_motorDB.open()) {
        qDebug()<<"Fail!";
        return false;
    }
    return true;
}

bool MotorDBManager::close()
{
    m_motorDB.close();
    return true;
}

void MotorDBManager::setMotorSeq(const QString &seq)
{
    QSqlTableModel model;
    model.setTable("sqlite_sequence");
    model.select();
    QSqlRecord record = model.record(1);
    record.setValue("seq", seq);
    model.setRecord(1, record);
    model.submitAll();
}

void MotorDBManager::setCompanySeq(const QString &seq)
{
    QSqlTableModel model;
    model.setTable("sqlite_sequence");
    model.select();
    QSqlRecord record = model.record(0);
    record.setValue("seq", seq);
    model.setRecord(0, record);
    model.submitAll();
}

