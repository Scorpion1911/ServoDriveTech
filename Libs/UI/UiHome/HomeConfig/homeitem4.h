#ifndef HOMEITEM4_H
#define HOMEITEM4_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem4 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem4(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem4();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM4_H
