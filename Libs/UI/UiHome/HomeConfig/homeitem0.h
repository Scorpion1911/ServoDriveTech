#ifndef HOMEITEM0_H
#define HOMEITEM0_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem0 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem0(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem0();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM0_H
