#ifndef CURVECUSACCACTIVATOR_H
#define CURVECUSACCACTIVATOR_H

#include <QObject>
#include <ctkPluginActivator.h>

class ICurve;

class CurveCusAccdActivator : public QObject, public ctkPluginActivator
{
    Q_OBJECT
    Q_INTERFACES(ctkPluginActivator)
    Q_PLUGIN_METADATA(IID "CurveCusAccd")
public:
    explicit CurveCusAccdActivator();
    void start(ctkPluginContext *context) Q_DECL_OVERRIDE;
    void stop(ctkPluginContext *context) Q_DECL_OVERRIDE;

signals:

public slots:
private:
    ICurve *m_iCurve;
};

#endif // CURVECUSACCACTIVATOR_H
