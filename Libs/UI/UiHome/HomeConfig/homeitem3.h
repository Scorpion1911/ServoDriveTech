#ifndef HOMEITEM3_H
#define HOMEITEM3_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem3 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem3(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem3();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM3_H
