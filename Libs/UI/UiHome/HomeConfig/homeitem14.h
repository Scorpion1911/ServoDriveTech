#ifndef HOMEITEM14_H
#define HOMEITEM14_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem14 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem14(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem14();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM14_H
