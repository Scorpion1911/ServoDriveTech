#ifndef GRAPHIOCONF138_H
#define GRAPHIOCONF138_H

#include <QWidget>
#include "igraphioconf.h"
#include "uiioconf_global.h"

namespace Ui {
class GraphIOConf138;
}

class GraphIOConf138Private;

class UIIOCONFSHARED_EXPORT GraphIOConf138 : public IGraphIOConf
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GraphIOConf138)

public:
    explicit GraphIOConf138(QWidget *parent = 0);
    ~GraphIOConf138();
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
    Ui::GraphIOConf138 *ui;
};

#endif // GRAPHIOCONF138_H
