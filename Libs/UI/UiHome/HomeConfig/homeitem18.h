#ifndef HOMEITEM18_H
#define HOMEITEM18_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem18 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem18(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem18();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM18_H
