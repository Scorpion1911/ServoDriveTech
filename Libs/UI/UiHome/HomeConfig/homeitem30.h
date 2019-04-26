#ifndef HOMEITEM30_H
#define HOMEITEM30_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem30 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem30(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem30();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM30_H
