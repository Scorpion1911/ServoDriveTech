#ifndef HOMEITEM35_H
#define HOMEITEM35_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem35 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem35(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem35();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM35_H
