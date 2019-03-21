#ifndef IGRAPHIOANALOG_P
#define IGRAPHIOANALOG_P

#include "igraphioanalog.h"
#include "igraph_p.h"
#include "iuiwidget_global.h"

class QGraphicsScene;
class WidgetItem;
class ArrowItem;
class AnchorItemHelper;
class QTimer;

class IUIWIDGETSHARED_EXPORT IGraphIOAnalogPrivate:public IGraphPrivate {
    Q_DECLARE_PUBLIC(IGraphIOAnalog)
public:
    IGraphIOAnalogPrivate(){}
    virtual ~IGraphIOAnalogPrivate(){}
protected:
    AnchorItemHelper *m_anchorHelper;
    QList<ArrowItem *> m_arrowList;
    QList<WidgetItem *> m_widgetItemList;
    QGraphicsScene* m_scene;
    WidgetItem* m_startText;
    WidgetItem* m_OfstWidget;
    WidgetItem* m_ScaleWidget;
    WidgetItem* m_CommandWidget;
    WidgetItem* m_endText;
    WidgetItem* m_TStart;
    WidgetItem* m_TEnd;

    QDoubleSpinBox *m_ofstEdit;
    QDoubleSpinBox *m_scaleEdit;
    QDoubleSpinBox *m_commandEdit;

    ArrowItem* m_A0;
    ArrowItem* m_A1;
    ArrowItem* m_A2;
    ArrowItem* m_A3;

    QColor m_arrowColor;
    QTimer *m_timer;
};

#endif // IGRAPHIOANALOG_P

