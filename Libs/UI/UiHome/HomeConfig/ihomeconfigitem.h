#ifndef IHOMECONFIGITEM_H
#define IHOMECONFIGITEM_H

#include <QObject>

class IHomeConfigItem : public QObject
{
    Q_OBJECT
public:
    explicit IHomeConfigItem(QObject *parent = 0);
    virtual ~IHomeConfigItem();
    virtual bool execute();

    int getHomeType();
    void setHomeType(int type);

    quint16 getHomeConfig();
    void setHomeConfig(quint16 config);

signals:

public slots:
protected:
    int m_homeType;
    quint16 m_homeConfig;


};

#endif // IHOMECONFIGITEM_H
