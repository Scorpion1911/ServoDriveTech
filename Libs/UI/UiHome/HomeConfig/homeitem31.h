#ifndef HOMEITEM31_H
#define HOMEITEM31_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem31 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem31(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem31();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM31_H
