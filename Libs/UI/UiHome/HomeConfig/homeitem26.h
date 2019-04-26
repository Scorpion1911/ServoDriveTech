#ifndef HOMEITEM26_H
#define HOMEITEM26_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem26 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem26(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem26();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM26_H
