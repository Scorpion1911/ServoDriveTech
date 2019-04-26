#ifndef HOMEITEM7_H
#define HOMEITEM7_H

#include <QObject>
#include "ihomeconfigitem.h"

class HomeItem7 : public IHomeConfigItem
{
    Q_OBJECT
public:
    explicit HomeItem7(int axisInx, SevDevice* dev, QObject *parent = 0);
    ~HomeItem7();

    bool execute() Q_DECL_OVERRIDE;

signals:

public slots:
};

#endif // HOMEITEM7_H
