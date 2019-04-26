#ifndef HOMEITEM8_H
#define HOMEITEM8_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem8 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem8(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem8();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM8_H
