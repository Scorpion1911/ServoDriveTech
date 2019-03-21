#ifndef HOMECONFIGBINDING139_H
#define HOMECONFIGBINDING139_H

#include <QObject>
#include "idatabinding.h"

class HomeConfigBinding139 : public IDataBinding
{
    Q_OBJECT
public:
    explicit HomeConfigBinding139(QObject *parent = 0);
    ~HomeConfigBinding139();

    void syncMultiUiDataToTree();
    void syncMultiTreeToUiData();

      //just sync treeitem <-> listWidget
    void syncUiDataToTreeItem();
    void syncTreeItemToUiData();

signals:

public slots:
};

#endif // HOMECONFIGBINDING139_H
