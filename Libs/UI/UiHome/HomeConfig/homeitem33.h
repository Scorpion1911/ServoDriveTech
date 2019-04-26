#ifndef HOMEITEM33_H
#define HOMEITEM33_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem33 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem33(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem33();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM33_H
