#ifndef HOMEITEM2_H
#define HOMEITEM2_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem2 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem2(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem2();

    bool execute() Q_DECL_OVERRIDE;
};

#endif // HOMEITEM2_H
