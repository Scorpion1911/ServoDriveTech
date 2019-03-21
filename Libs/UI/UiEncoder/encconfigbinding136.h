#ifndef ENCCONFIGBINDING136_H
#define ENCCONFIGBINDING136_H

#include <QObject>
#include "idatabinding.h"

class EncConfigBinding136 : public IDataBinding
{
  Q_OBJECT
public:
  explicit EncConfigBinding136(QObject *parent = 0);
  ~EncConfigBinding136();
  void syncMultiUiDataToTree();
  void syncMultiTreeToUiData();

  //just sync treeitem <-> listWidget
  void syncUiDataToTreeItem();
  void syncTreeItemToUiData();

signals:

public slots:
};

#endif // ENCCONFIGBINDING_H
