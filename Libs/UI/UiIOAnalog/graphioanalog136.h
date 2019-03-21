#ifndef GRAPHIOANALOG136_H
#define GRAPHIOANALOG136_H

#include <QWidget>
#include "uiioanalog_global.h"
#include "igraphioanalog.h"

namespace Ui {
class GraphIOAnalog136;
}

class GraphIOAnalog136Private;
class QDoubleSpinBox;
class QTreeWidgetItem;

class UIIOANALOGSHARED_EXPORT GraphIOAnalog136 : public IGraphIOAnalog
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphIOAnalog136)
public:
    explicit GraphIOAnalog136(QWidget *parent = 0);
    ~GraphIOAnalog136();    
    void syncTreeDataToUiFace() Q_DECL_OVERRIDE;
    void setParaScale() Q_DECL_OVERRIDE;
    void setTimerActive(bool en) Q_DECL_OVERRIDE;

protected:
    void setCustomVisitActive(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
    void setUiVersionName() Q_DECL_OVERRIDE;
    void setupDataMappings() Q_DECL_OVERRIDE;
    QPointF ofstInitPos();
private:
    void createInputItems();
    void createOutputItems();
private slots:
    void onTimeOut();

private:
    Ui::GraphIOAnalog136 *ui;
};

#endif // GRAPHIOANALOG136_H
