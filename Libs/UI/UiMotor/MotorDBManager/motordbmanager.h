#ifndef MOTORDBMANAGER_H
#define MOTORDBMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>

class MotorDBManager : public QObject
{
    Q_OBJECT
public:
    explicit MotorDBManager(const QString &dbPath, const QString &userName, const QString &psw, QObject *parent = 0);
    ~MotorDBManager();
    QList<int> getCompanyIdList();
    QString getCompanyName(int id);
    QList<int> getMotorIdList(int id);
    QString getMotorName(int id);
    double getMotorPara(int id, const QString &paraName);
    void addMotor(const QStringList &list);
    void removeMotor(int motorId);
    void changeMotor(const QStringList &list);
    int getMotorSeq();
    int getCompanySeq();
    bool open();
    bool close();

signals:

public slots:
private:
    void setMotorSeq(const QString &seq);
    void setCompanySeq(const QString &seq);
private:
    QSqlDatabase m_motorDB;
    QString m_dbPath;
    QString m_usrName;
    QString m_psw;
};

#endif // MOTORDBMANAGER_H
