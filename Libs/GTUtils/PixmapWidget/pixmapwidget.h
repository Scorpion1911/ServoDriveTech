#ifndef PIXMAPWIDGET_H
#define PIXMAPWIDGET_H

#include <QWidget>
#include "gtutils_global.h"

class PixmapWidgetPrivate;

class GTUTILSSHARED_EXPORT PixmapWidget : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PixmapWidget)
public:
    explicit PixmapWidget(PixmapWidgetPrivate *pri_ptr, QString picPath, QWidget *parent = 0);
    ~PixmapWidget();
    bool init();
    bool setMiniSizeIsScreenTime(double time);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

signals:

public slots:

private:
    PixmapWidgetPrivate *d_ptr;
};

#endif // PIXMAPWIDGET_H
