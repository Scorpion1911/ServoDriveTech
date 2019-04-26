#ifndef HOMEITEM19_H
#define HOMEITEM19_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem19 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem19(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem19();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM19_H
