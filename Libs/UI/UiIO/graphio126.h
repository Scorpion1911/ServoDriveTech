#ifndef GRAPHIO126_H
#define GRAPHIO126_H

#include "uiio_global.h"
#include "igraphio.h"
#include "optface.h"

#include <QWidget>

namespace Ui {
class GraphIO126;
}

class GraphIO126Private;
class QCheckBox;

class UIIOSHARED_EXPORT GraphIO126 : public IGraphIO
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphIO126)

public:
    explicit GraphIO126(QWidget *parent = 0);
    ~GraphIO126();

    void syncTreeDataToUiFace() Q_DECL_OVERRIDE;
    void initUi() Q_DECL_OVERRIDE;
    void visit(IUiWidget *uiWidget) Q_DECL_OVERRIDE;

protected:
    void setCustomVisitActive(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
    void setUiVersionName() Q_DECL_OVERRIDE;
    void setupDataMappings() Q_DECL_OVERRIDE;
    void initPrivateData(IUiWidget *uiWidget) Q_DECL_OVERRIDE;

protected slots:
    void onCheckBoxStateChanged(int axisIndex);

private:
    void setBoxStyleSheet(QCheckBox *box);
    quint32 operationValue(quint32 bitstart, quint32 bitwidth);
private:
    Ui::GraphIO126 *ui;
};

#endif // GRAPHIO126_H
