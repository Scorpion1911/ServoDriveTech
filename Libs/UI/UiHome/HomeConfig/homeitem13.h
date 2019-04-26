#ifndef HOMEITEM13_H
#define HOMEITEM13_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem13 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem13(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem13();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM13_H
