#ifndef IGRAPHIOCONF_H
#define IGRAPHIOCONF_H

#include <QWidget>
#include "iuiwidget_global.h"
#include "igraph.h"

class IGraphIOConfPrivate;
class IUiWidget;

#include <QObject>

class IUIWIDGETSHARED_EXPORT IGraphIOConf : public IGraph
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IGraphIOConf)
public:
//    explicit IGraphIOConf(QObject *parent = 0);
    virtual ~IGraphIOConf();
    void virtual initUi() = 0;
    virtual void visit(IUiWidget *uiWidget) Q_DECL_OVERRIDE = 0;
    virtual void setTimerActive(bool en) = 0;
    virtual void syncUiToTree() = 0;
protected:
    virtual void setUiVersionName() Q_DECL_OVERRIDE = 0;
    virtual void setCustomVisitActive(IUiWidget*uiWidget) Q_DECL_OVERRIDE = 0;

signals:

public slots:
protected slots:
protected:
    IGraphIOConf(IGraphIOConfPrivate &dd, QWidget *parent = 0);
};

#endif // IGRAPHIOCONF_H
