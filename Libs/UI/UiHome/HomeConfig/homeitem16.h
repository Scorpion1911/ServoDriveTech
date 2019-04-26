#ifndef HOMEITEM16_H
#define HOMEITEM16_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem16 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem16(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem16();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM16_H
