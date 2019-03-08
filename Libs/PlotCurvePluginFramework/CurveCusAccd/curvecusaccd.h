#ifndef CURVECUSACCD_H
#define CURVECUSACCD_H

#include <QObject>
#include "icurve.h"

class ctkPluginContext;

class CurveCusAccd : public QObject, public ICurve
{
    Q_OBJECT
    Q_INTERFACES(ICurve)

public:
    CurveCusAccd();
    CurveCusAccd(ctkPluginContext *context);

    ICurve *clone() Q_DECL_OVERRIDE ;
    void init() Q_DECL_OVERRIDE ;
    void reset() {}

    void calculate() Q_DECL_OVERRIDE ;

    QString name() Q_DECL_OVERRIDE ;
    QString note() Q_DECL_OVERRIDE ;
    QString pluginName() Q_DECL_OVERRIDE ;
};

#endif // CURVECUSACC_H
