#ifndef HOMEITEM32_H
#define HOMEITEM32_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem32 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem32(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem32();

    bool execute() Q_DECL_OVERRIDE;
};

#endif // HOMEITEM32_H
