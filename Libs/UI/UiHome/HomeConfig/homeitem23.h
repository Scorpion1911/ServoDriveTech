#ifndef HOMEITEM23_H
#define HOMEITEM23_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem23 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem23(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem23();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM23_H
