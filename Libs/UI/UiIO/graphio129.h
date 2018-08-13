#ifndef GRAPHIO129_H
#define GRAPHIO129_H

#include "uiio_global.h"
#include "igraphio.h"
#include "optface.h"

#include <QWidget>

namespace Ui {
class GraphIO129;
}

class GraphIO129Private;
class QCheckBox;

class UIIOSHARED_EXPORT GraphIO129 : public IGraphIO
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphIO129)

public:
    explicit GraphIO129(QWidget *parent = 0);
    ~GraphIO129();

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
    Ui::GraphIO129 *ui;

};

#endif // GRAPHIO129_H
