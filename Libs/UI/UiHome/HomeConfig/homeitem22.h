#ifndef HOMEITEM22_H
#define HOMEITEM22_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem22 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem22(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem22();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM22_H
