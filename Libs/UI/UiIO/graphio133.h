#ifndef GRAPHIO133_H
#define GRAPHIO133_H

#include "uiio_global.h"
#include "igraphio.h"
#include "optface.h"

#include <QWidget>

namespace Ui {
class GraphIO133;
}

class GraphIO133Private;
class QCheckBox;

class UIIOSHARED_EXPORT GraphIO133 : public IGraphIO
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphIO133)

public:
    explicit GraphIO133(QWidget *parent = 0);
    ~GraphIO133();

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
private:
    Ui::GraphIO133 *ui;

};

#endif // GRAPHIO133_H
