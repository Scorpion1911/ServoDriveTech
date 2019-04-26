#ifndef ENCCONFIGHECHUANITEM_H
#define ENCCONFIGHECHUANITEM_H

#include <QObject>
#include "iencconfigitem.h"

class EncConfigHeChuanItem : public IEncConfigItem
{
    Q_OBJECT
public:
    explicit EncConfigHeChuanItem(QObject *parent = 0);
    ~EncConfigHeChuanItem();

signals:

public slots:
};

#endif // ENCCONFIGHECHUANITEM_H
