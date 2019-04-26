#ifndef HOMEITEM34_H
#define HOMEITEM34_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem34 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem34(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem34();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM34_H
