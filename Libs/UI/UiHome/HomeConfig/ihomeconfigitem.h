#ifndef IHOMECONFIGITEM_H
#define IHOMECONFIGITEM_H

#include <QObject>

#define    POSITIVE_LIMIT_ADDR    13912
#define    NEGATIVE_LIMIT_ADDR    13913
#define    HOME_SWITCH_ADDR       13914
#define    INPUT_REVERSE_ADDR     28693

class SevDevice;

class IHomeConfigItem : public QObject
{
    Q_OBJECT
public:
    explicit IHomeConfigItem(int axisInx, SevDevice* dev, QObject *parent = 0);
    virtual ~IHomeConfigItem();
    virtual bool execute();

    int getHomeType();
    void setHomeType(int type);

    quint16 getHomeConfig();
    void setHomeConfig(quint16 config);
    
    bool getReverse();
    void setReverse(bool en);

signals:

public slots:
protected:
    int m_homeType;
    quint16 m_homeConfig;
    bool m_reverse;
    int m_axisInx;
    SevDevice* m_dev;

};

#endif // IHOMECONFIGITEM_H
