#ifndef GRAPHHOME136_H
#define GRAPHHOME136_H

#include <QWidget>
#include "uihome_global.h"
#include "igraphhome.h"

namespace Ui {
class GraphHome136;
}

class GraphHome136Private;
class QDoubleSpinBox;
class QTreeWidgetItem;

class UIHOMESHARED_EXPORT GraphHome136 : public IGraphHome
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphHome136)

public:
    explicit GraphHome136(QWidget *parent = 0);
    ~GraphHome136();
    void syncTreeDataToUiFace() Q_DECL_OVERRIDE;
public slots:
    void onBtnHomeConfigSaveClicked() Q_DECL_OVERRIDE;

protected:
    void setCustomVisitActive(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
    void setUiVersionName() Q_DECL_OVERRIDE;
    void setupDataMappings() Q_DECL_OVERRIDE;
private:
    void createHomeItems();
    void initCurrentHomeItem();
    void updateUiByCurrentItem();
private:
    Ui::GraphHome136 *ui;
};

#endif // GRAPHHOME136_H
