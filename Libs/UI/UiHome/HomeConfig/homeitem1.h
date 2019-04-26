#ifndef HOMEITEM1_H
#define HOMEITEM1_H

#include <QObject>
#include "ihomeconfigitem.h"


class HomeItem1 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem1(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem1();

    bool execute() Q_DECL_OVERRIDE;
};

#endif // HOMEITEM1_H
