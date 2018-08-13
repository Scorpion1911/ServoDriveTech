#ifndef IGRAPHIO_H
#define IGRAPHIO_H

#include <QWidget>
#include "igraph.h"
#include "iuiwidget_global.h"

class IGraphIOPrivate;
class IUiWidget;

typedef enum {
  TREE_IO_NAME_COL = 0,
  TREE_IO_VALUE_COL,
  TREE_IO_MEMTYPE_COL,
  TREE_IO_ADDR_COL,
  TREE_IO_BASEADDR_COL,
  TREE_IO_BITSTART_COL,
  TREE_IO_BITWIDTH_COL,
  TREE_IO_CONTROLTYPE_COL,
  TREE_IO_IOTYPE_COL,
  TREE_IO_IOTYPENAME_COL,
  TREE_IO_PHYPIN_COL,
  TREE_IO_NOTE_COL,
  TREE_IO_TOTAL_COL
}Tree_IO_Column_Index;

class IUIWIDGETSHARED_EXPORT IGraphIO : public IGraph
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IGraphIO)
public:
//    explicit IGraphIO(QObject *parent = 0);
    virtual ~IGraphIO();
    void virtual initUi() = 0;
    virtual void visit(IUiWidget *uiWidget) Q_DECL_OVERRIDE = 0;
protected:
    virtual void setUiVersionName() Q_DECL_OVERRIDE = 0;
    virtual void setCustomVisitActive(IUiWidget*uiWidget) Q_DECL_OVERRIDE = 0;
    virtual void setupDataMappings() = 0;
    void initPrivateData(IUiWidget *uiWidget) Q_DECL_OVERRIDE = 0;

signals:

public slots:
protected slots:
protected:
    IGraphIO(IGraphIOPrivate &dd, QWidget *parent = 0);
};

#endif // IGRAPHIO_H
