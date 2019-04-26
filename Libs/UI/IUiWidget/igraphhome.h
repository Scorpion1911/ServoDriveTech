#ifndef IGRAPHHOME_H
#define IGRAPHHOME_H

#include <QWidget>
#include "igraph.h"
#include "iuiwidget_global.h"

class IGraphHomePrivate;
class IUiWidget;

class IUIWIDGETSHARED_EXPORT IGraphHome : public IGraph
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IGraphHome)
public:
    //explicit IGraphHome(QWidget *parent = 0);
    virtual ~IGraphHome();

signals:

public slots:
    virtual void onBtnHomeConfigSaveClicked() = 0;
protected:
    virtual void setUiVersionName() Q_DECL_OVERRIDE = 0;
    virtual void setCustomVisitActive(IUiWidget*uiWidget) Q_DECL_OVERRIDE = 0;
    virtual void setupDataMappings() = 0;
    IGraphHome(IGraphHomePrivate &dd, QWidget *parent = 0);
};

#endif // IGRAPHHOME_H
