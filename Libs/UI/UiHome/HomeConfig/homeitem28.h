#ifndef HOMEITEM28_H
#define HOMEITEM28_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem28 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem28(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem28();

    bool execute() Q_DECL_OVERRIDE;
};

#endif // HOMEITEM28_H
