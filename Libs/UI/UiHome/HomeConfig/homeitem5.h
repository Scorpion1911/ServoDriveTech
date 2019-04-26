#ifndef HOMEITEM5_H
#define HOMEITEM5_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem5 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem5(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem5();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM5_H
