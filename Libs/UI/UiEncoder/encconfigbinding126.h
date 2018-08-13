#ifndef ENCCONFIGBINDING126_H
#define ENCCONFIGBINDING126_H

#include <QObject>
#include "idatabinding.h"

class EncConfigBinding126 : public IDataBinding
{
  Q_OBJECT
public:
  explicit EncConfigBinding126(QObject *parent = 0);
  ~EncConfigBinding126();
  void syncMultiUiDataToTree();
  void syncMultiTreeToUiData();

  //just sync treeitem <-> listWidget
  void syncUiDataToTreeItem();
  void syncTreeItemToUiData();

signals:

public slots:
};

#endif // ENCCONFIGBINDING_H
