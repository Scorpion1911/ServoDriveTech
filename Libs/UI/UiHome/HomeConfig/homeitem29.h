#ifndef HOMEITEM29_H
#define HOMEITEM29_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem29 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem29(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem29();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM29_H
