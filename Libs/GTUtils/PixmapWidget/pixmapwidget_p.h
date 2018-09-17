#ifndef PIXMAPWIDGET_P_H
#define PIXMAPWIDGET_P_H

#include <QWidget>
#include "pixmapwidget.h"
#include <QString>
#include <QPixmap>
#include "gtutils_global.h"

class GTUTILSSHARED_EXPORT PixmapWidgetPrivate
{
    Q_DECLARE_PUBLIC(PixmapWidget)
public:
    PixmapWidgetPrivate();
    ~PixmapWidgetPrivate();

    PixmapWidget *q_ptr;
    QString m_picPath;
    double m_widthScale;
    double m_heightScale;
    double m_oldWidthScale;
    double m_oldHeightScale;
    double m_maxWidthScale;
    double m_maxHeightScale;
    double m_miniScale;
    double m_miniSizeIsScreenTime;

    QPixmap m_pixmap;
    int m_pixmapWidth;
    int m_pixmapHeight;

    int m_widgetWidth;
    int m_widgetHeight;
};



#endif // PIXMAPWIDGET_P_H

