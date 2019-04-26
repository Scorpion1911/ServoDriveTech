#ifndef HOMEITEM11_H
#define HOMEITEM11_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem11 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem11(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem11();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM11_H
