#include "pixmapwidget.h"
#include "pixmapwidget_p.h"

//#include "optcontainer.h"
//#include "optface.h"
#include <QString>
#include "gtutils.h"
#include <QPixmap>
#include <QPainter>
#include <QPoint>
#include <QWheelEvent>
#include <QDebug>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>

#define RATIO_MOUSE_WHEEL (8*360)

PixmapWidgetPrivate::PixmapWidgetPrivate()
{

}

PixmapWidgetPrivate::~PixmapWidgetPrivate()
{

}

PixmapWidget::PixmapWidget(PixmapWidgetPrivate *pri_ptr, QString picPath, QWidget *parent) : QWidget(parent), d_ptr(pri_ptr)
{
    d_ptr->q_ptr = this;
    d_ptr->m_picPath = picPath;
    d_ptr->m_miniSizeIsScreenTime = 0.1;
    init();

    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
//    this->setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

PixmapWidget::~PixmapWidget()
{
    delete d_ptr;
}

bool PixmapWidget::init()
{
    Q_D(PixmapWidget);
//    OptFace *face = dynamic_cast<OptFace *>(OptContainer::instance()->optItem("optface"));
//    QString picPath = GTUtils::customPath() + "option/style/" + face->css() + "/icon/" + d->m_picPath;
//    d->m_pixmap.load(picPath);
    d->m_pixmap.load(d->m_picPath);
    d->m_pixmapWidth = d->m_pixmap.width();
    d->m_pixmapHeight = d->m_pixmap.height();

    QRect desktop = QApplication::desktop()->screenGeometry();
    int screenWidth = desktop.width();
    int screenHeight = desktop.height();
    qDebug()<<"screenWidth:"<<screenWidth;
    qDebug()<<"screenHeight:"<<screenHeight;

    d->m_maxWidthScale = screenWidth / ((double)d->m_pixmapWidth);
    d->m_maxHeightScale = screenHeight / ((double)d->m_pixmapHeight);

    //根据屏幕的像素设置窗口缩放的最小值
    double miniWidthSizeByScreen = screenWidth * d->m_miniSizeIsScreenTime;
    double miniHeightSizeByScreen = screenHeight * d->m_miniSizeIsScreenTime;
    double miniWidthScale = miniWidthSizeByScreen / d->m_pixmapWidth;
    double miniHeightScale = miniHeightSizeByScreen / d->m_pixmapHeight;
    if(miniWidthScale < miniHeightScale){
        d->m_miniScale = miniWidthScale;
    }else{
        d->m_miniScale = miniHeightScale;
    }
    this->setMinimumSize(d->m_miniScale * d->m_pixmapWidth, d->m_miniScale * d->m_pixmapHeight);

    //设置窗口的初始大小，若是图片大于屏幕像素，则按图片原本比例设置最大窗口大小
    if(d->m_maxWidthScale < 1.0 || d->m_maxHeightScale < 1.0){
        double maxScale;
        if(d->m_maxWidthScale < d->m_maxHeightScale)
            maxScale = d->m_maxWidthScale;
        else
            maxScale = d->m_maxHeightScale;
        this->resize(maxScale * d->m_pixmapWidth, maxScale * d->m_pixmapHeight);
    }else if(d->m_miniScale > 1.0){
        this->resize(d->m_miniScale * d->m_pixmapWidth, d->m_miniScale * d->m_pixmapHeight);
    }else{
        this->resize(d->m_pixmapWidth, d->m_pixmapHeight);
    }

//    if(d->m_maxWidthScale > 1.0)
//        d->m_widthScale = 1.0;
//    else
//        d->m_widthScale = d->m_maxWidthScale;

//    if(d->m_maxHeightScale > 1.0)
//        d->m_heightScale = 1.0;
//    else
//        d->m_heightScale = d->m_maxHeightScale;

    return true;
}

bool PixmapWidget::setMiniSizeIsScreenTime(double time)
{
    Q_D(PixmapWidget);
    if(time > 1.0){
        return false;
    }else{
        d->m_miniSizeIsScreenTime = time;
        init();
    }
    return true;
}

void PixmapWidget::paintEvent(QPaintEvent *event)
{
    Q_D(PixmapWidget);
//    if(d->m_widgetWidth != this->width() && d->m_widgetHeight == this->height()){
//        d->m_scale = this->width()/(double)d->m_pixmapWidth;
//    }else if(d->m_widgetWidth == this->width() && d->m_widgetHeight != this->height()){
//        d->m_scale = this->height()/(double)d->m_pixmapHeight;
//    }else{
//        double widthScale = this->width()/(double)d->m_pixmapWidth;
//        double heightScale = this->height()/(double)d->m_pixmapHeight;
//        if(d->m_widgetWidth < this->width() && d->m_widgetHeight < this->height()){
//            if(widthScale > heightScale){
//                d->m_scale = widthScale;
//            }else{
//                d->m_scale = heightScale;
//            }
//        }
//        if(d->m_widgetWidth > this->width() && d->m_widgetHeight > this->height()){
//            if(widthScale < heightScale){
//                d->m_scale = widthScale;
//            }else{
//                d->m_scale = heightScale;
//            }
//        }
//    }

    //1.改变窗口的方式来填充图片
    if(d->m_widgetWidth != this->width() || d->m_widgetHeight != this->height()){
        d->m_widthScale = this->width()/(double)d->m_pixmapWidth;
        d->m_heightScale = this->height()/(double)d->m_pixmapHeight;
    }

    //限制比例值的大小，一旦某（高、宽）比例值超过规定的最大值，将赋予该比例值最大值，允许另一比例值改变
    if(d->m_widthScale > d->m_maxWidthScale){
        d->m_widthScale = d->m_maxWidthScale;
    }else if(d->m_heightScale > d->m_maxHeightScale){
        d->m_heightScale = d->m_maxHeightScale;
    }
    d->m_oldWidthScale = d->m_widthScale;
    d->m_oldHeightScale = d->m_heightScale;


    int painterWidth = d->m_pixmapWidth * d->m_widthScale;
    int painterHeight = d->m_pixmapHeight * d->m_heightScale;
//    QPoint centerPoint(this->pos().x() + this->width()/2,
//                       this->pos().y() + this->height()/2);
//    this->move(centerPoint.x() - painterWidth/2,
//               centerPoint.y() - painterHeight/2);
    this->resize(painterWidth, painterHeight);
    QPoint painterStartPos(0 + this->width()/2 - painterWidth/2,
                            0 + this->height()/2 - painterHeight/2);
    QPainter painter(this);
    painter.drawPixmap(QRect(painterStartPos.x(), painterStartPos.y(), painterWidth, painterHeight), d->m_pixmap);

    d->m_widgetWidth = this->width();
    d->m_widgetHeight = this->height();

    QWidget::paintEvent(event);
}

//2.通过鼠标滑轮的滚动的方式填充图片
void PixmapWidget::wheelEvent(QWheelEvent *event)
{
    Q_D(PixmapWidget);
    int value = event->delta();
    double scaleRatio = ((double)value)/RATIO_MOUSE_WHEEL;
    d->m_widthScale += scaleRatio;
    d->m_heightScale = (d->m_widthScale*d->m_oldHeightScale)/d->m_oldWidthScale;//等比例运算
    qDebug()<<"value:"<<value;
    qDebug()<<"scaleRatio:"<<scaleRatio;

    //限制比例值的大小，一旦某（高、宽）比例值超过规定的最大值，高、宽比例值都保持不变
    if(d->m_widthScale > d->m_maxWidthScale || d->m_heightScale > d->m_maxHeightScale
           || d->m_widthScale < d->m_miniScale || d->m_heightScale < d->m_miniScale){
        d->m_widthScale = d->m_oldWidthScale;
        d->m_heightScale = d->m_oldHeightScale;
    }
    d->m_oldWidthScale = d->m_widthScale;
    d->m_oldHeightScale = d->m_heightScale;

    qDebug()<<"d->m_widthScale:"<<d->m_widthScale;
    qDebug()<<"d->m_heightScale:"<<d->m_heightScale;

    update();

    event->accept();
}

