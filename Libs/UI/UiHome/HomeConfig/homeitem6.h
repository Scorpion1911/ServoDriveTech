#ifndef HOMEITEM6_H
#define HOMEITEM6_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem6 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem6(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem6();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM6_H
