#ifndef HOMECONFIGMANAGER_H
#define HOMECONFIGMANAGER_H

#include <QObject>

class IHomeConfigItem;

class HomeConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit HomeConfigManager(QObject *parent = 0);
    ~HomeConfigManager();
    void addHomeItem(IHomeConfigItem *item);
    IHomeConfigItem *homeItem(quint8 index);
    QStringList itemNames();
    void clearAllHomeItems();

    //QWidget *curAttributeWidget() const;
    //void setCurAttributeWidget(QWidget *w);

signals:

public slots:
private:
    QList<IHomeConfigItem*> m_homeItemList;
    //QWidget *mp_curAttributeWidget;
};

#endif // HOMECONFIGMANAGER_H
