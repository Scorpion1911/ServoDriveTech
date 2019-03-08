#ifndef ENCCONFIGABZ_H
#define ENCCONFIGABZ_H

#include <QObject>
#include "iencconfigitem.h"

class EncConfigABZ : public IEncConfigItem
{
    Q_OBJECT
public:
    explicit EncConfigABZ(QObject *parent = 0);
    ~EncConfigABZ();

signals:

public slots:
};

#endif // ENCCONFIGABZ_H
