#ifndef HOMEITEM12_H
#define HOMEITEM12_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem12 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem12(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem12();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM12_H
