#ifndef HOMEITEM9_H
#define HOMEITEM9_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem9 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem9(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem9();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM9_H
