#ifndef HOMEITEM24_H
#define HOMEITEM24_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem24 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem24(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem24();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM24_H
