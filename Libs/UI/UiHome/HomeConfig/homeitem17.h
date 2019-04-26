#ifndef HOMEITEM17_H
#define HOMEITEM17_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem17 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem17(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem17();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM17_H
