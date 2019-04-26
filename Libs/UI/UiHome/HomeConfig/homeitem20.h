#ifndef HOMEITEM20_H
#define HOMEITEM20_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem20 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem20(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem20();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM20_H
