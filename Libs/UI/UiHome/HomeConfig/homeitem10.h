#ifndef HOMEITEM10_H
#define HOMEITEM10_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem10 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem10(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem10();

    bool execute() Q_DECL_OVERRIDE;
};

#endif // HOMEITEM10_H
