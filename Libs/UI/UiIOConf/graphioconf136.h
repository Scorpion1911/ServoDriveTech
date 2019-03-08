#ifndef GRAPHIOCONF136_H
#define GRAPHIOCONF136_H

#include <QWidget>
#include "igraphioconf.h"
#include "uiioconf_global.h"

namespace Ui {
class GraphIOConf136;
}

class GraphIOConf136Private;

class UIIOCONFSHARED_EXPORT GraphIOConf136 : public IGraphIOConf
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphIOConf136)

public:
    explicit GraphIOConf136(QWidget *parent = 0);
    ~GraphIOConf136();
    void syncTreeDataToUiFace() Q_DECL_OVERRIDE;
    void syncUiToTree() Q_DECL_OVERRIDE;
    void initUi() Q_DECL_OVERRIDE;
    void visit(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
    void setTimerActive(bool en) Q_DECL_OVERRIDE;

protected:
    void setCustomVisitActive(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
    void setUiVersionName() Q_DECL_OVERRIDE;    
    void setupDataMappings() Q_DECL_OVERRIDE;
private:
    bool checkFuncSetting();
private slots:
    void onTimeOut();
    void onEnStatusChanged(int index, bool en);

private:
    Ui::GraphIOConf136 *ui;
};

#endif // GRAPHIOCONF136_H
