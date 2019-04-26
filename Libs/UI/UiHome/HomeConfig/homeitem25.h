#ifndef HOMEITEM25_H
#define HOMEITEM25_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem25 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem25(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem25();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM25_H
