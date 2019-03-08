#ifndef IGRAPHIOCONF_P
#define IGRAPHIOCONF_P

#include "igraphioconf.h"
#include "igraph_p.h"
#include "iuiwidget_global.h"

class IUIWIDGETSHARED_EXPORT IGraphIOConfPrivate:public IGraphPrivate {
    Q_DECLARE_PUBLIC(IGraphIOConf)
public:
    IGraphIOConfPrivate():m_timer(NULL){}
    virtual ~IGraphIOConfPrivate(){}
protected:
    QTimer *m_timer;
};

#endif // IGRAPHIOCONF_P

