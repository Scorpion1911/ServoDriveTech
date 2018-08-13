#ifndef IGRAPHIO_P
#define IGRAPHIO_P

#include "igraphio.h"
#include "igraph_p.h"
#include "iuiwidget_global.h"

class IUIWIDGETSHARED_EXPORT IGraphIOPrivate:public IGraphPrivate {
    Q_DECLARE_PUBLIC(IGraphIO)
public:
    IGraphIOPrivate(){}
    virtual ~IGraphIOPrivate(){}
protected:

};

#endif // IGRAPHIO_P

