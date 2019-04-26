#ifndef HOMEITEM27_H
#define HOMEITEM27_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem27 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem27(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem27();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM27_H
