#ifndef ENCCONFIGBINDING130_H
#define ENCCONFIGBINDING130_H

#include <QObject>
#include "idatabinding.h"

class EncConfigBinding130 : public IDataBinding
{
  Q_OBJECT
public:
  explicit EncConfigBinding130(QObject *parent = 0);
  ~EncConfigBinding130();
  void syncMultiUiDataToTree();
  void syncMultiTreeToUiData();

  //just sync treeitem <-> listWidget
  void syncUiDataToTreeItem();
  void syncTreeItemToUiData();

signals:

public slots:
};

#endif // ENCCONFIGBINDING_H
