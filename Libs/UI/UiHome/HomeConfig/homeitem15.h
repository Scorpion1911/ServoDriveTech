#ifndef HOMEITEM15_H
#define HOMEITEM15_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem15 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem15(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem15();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM15_H
