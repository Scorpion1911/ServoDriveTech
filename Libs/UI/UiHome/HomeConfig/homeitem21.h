#ifndef HOMEITEM21_H
#define HOMEITEM21_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem21 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem21(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem21();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM21_H
