#include "igraphioanalog.h"
#include "boxitemmapping.h"
#include "iuiwidget.h"
#include "igraphioanalog_p.h"
#include "widgetitem.h"
#include "arrowitem.h"
#include "labelitemwidget.h"
#include "anchoritemhelper.h"
#include "targetitemwidget.h"
#include "gtutils.h"
#include "optface.h"
#include "sevdevice.h"

#include <QGraphicsScene>
#include <QDoubleSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

IGraphIOAnalog::~IGraphIOAnalog() {
    Q_D(IGraphIOAnalog);
    foreach (WidgetItem *item, d->m_widgetItemList) {
        d->m_scene->removeItem(item);
        delete item;
        item = NULL;
    }
    d->m_widgetItemList.clear();

    foreach (ArrowItem *arrow, d->m_arrowList) {
        delete arrow;
        arrow = NULL;
    }
    d->m_arrowList.clear();

    qDebug()<<"IGraphIOAnalog destruct-->";
}

QColor IGraphIOAnalog::arrowColor() const
{
    Q_D(const IGraphIOAnalog);
    return d->m_arrowColor;
}

void IGraphIOAnalog::setArrowColor(const QColor &color)
{
    Q_D(IGraphIOAnalog);
    d->m_arrowColor=color;
    emit arrowColorChanged(color);
}

void IGraphIOAnalog::createStartText()
{
    Q_D(IGraphIOAnalog);

    LabelItemWidget *label = new LabelItemWidget(tr("Signal"));
    label->setObjectName("label_analogIOStartText");
    d->m_startText = new WidgetItem;
    d->m_startText->setWidget(label);
    d->m_startText->setObjectName("StartText");
    d->m_scene->addItem(d->m_startText);

    d->m_widgetItemList.append(d->m_startText);
}

void IGraphIOAnalog::createOffsetWidget()
{
    Q_D(IGraphIOAnalog);

    QWidget *wOfst = new QWidget;
    wOfst->setObjectName("widget_analogIOOffset");
    QVBoxLayout *vlayoutTest = new QVBoxLayout(wOfst);
    QLabel *title = new QLabel(tr("Analog Offset (V)"), wOfst);
    title->setObjectName("label_ofstTitle");
    title->setAlignment(Qt::AlignCenter);
    vlayoutTest->addWidget(title);

    QDoubleSpinBox *ofstEdit = new QDoubleSpinBox(wOfst);
    d->m_ofstEdit = ofstEdit;
    ofstEdit->setObjectName("dspinBox_analogioOfst");
    ofstEdit->setMinimum(-10);
    ofstEdit->setMaximum(10);
    ofstEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    vlayoutTest->addWidget(ofstEdit);

    wOfst->setLayout(vlayoutTest);

    d->m_OfstWidget = new WidgetItem;
    d->m_OfstWidget->setWidget(wOfst, true);
    d->m_OfstWidget->setObjectName("Ofst");
    d->m_scene->addItem(d->m_OfstWidget);//take ownership

    d->m_widgetItemList.append(d->m_OfstWidget);
}

void IGraphIOAnalog::createVoltageScale()
{
    Q_D(IGraphIOAnalog);

    QWidget *wScale = new QWidget;
    wScale->setObjectName("widget_analogIOScale");
    QVBoxLayout *vlayoutTest = new QVBoxLayout(wScale);
    QHBoxLayout *hlayoutTest = new QHBoxLayout;
    QLabel *title = new QLabel(tr("Voltage Scaling"), wScale);
    title->setObjectName("label_scaleTitle");
    title->setAlignment(Qt::AlignCenter);
    vlayoutTest->addWidget(title);

    QDoubleSpinBox *scaleEdit = new QDoubleSpinBox(wScale);
    d->m_scaleEdit = scaleEdit;
    scaleEdit->setObjectName("dspinBox_analogioScale");
    scaleEdit->setMinimum(0);
    scaleEdit->setMaximum(32767);
    scaleEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    QLabel *leftLabel = new QLabel("1V = ");
    QLabel *rightLabel = new QLabel("rpm");
    hlayoutTest->addWidget(leftLabel);
    hlayoutTest->addWidget(scaleEdit);
    hlayoutTest->addWidget(rightLabel);
    vlayoutTest->addLayout(hlayoutTest);

    wScale->setLayout(vlayoutTest);

    d->m_ScaleWidget = new WidgetItem;
    d->m_ScaleWidget->setWidget(wScale, true);
    d->m_ScaleWidget->setObjectName("Scale");
    d->m_scene->addItem(d->m_ScaleWidget);//take ownership

    d->m_widgetItemList.append(d->m_ScaleWidget);
}

void IGraphIOAnalog::createVoltageCommand()
{
    Q_D(IGraphIOAnalog);

    QWidget *wCommand = new QWidget;
    wCommand->setObjectName("widget_analogIOCommand");
    QVBoxLayout *vlayoutTest = new QVBoxLayout(wCommand);
    QLabel *title = new QLabel(tr("Voltage Command (V)"), wCommand);
    title->setObjectName("label_commandTitle");
    title->setAlignment(Qt::AlignCenter);
    vlayoutTest->addWidget(title);

    QDoubleSpinBox *commandEdit = new QDoubleSpinBox(wCommand);
    d->m_commandEdit = commandEdit;
    commandEdit->setObjectName("dspinBox_analogioCommand");
    commandEdit->setMinimum(-10);
    commandEdit->setMaximum(10);
    commandEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    commandEdit->setReadOnly(true);
    vlayoutTest->addWidget(commandEdit);

    wCommand->setLayout(vlayoutTest);

    d->m_CommandWidget = new WidgetItem;
    d->m_CommandWidget->setWidget(wCommand, true);
    d->m_CommandWidget->setObjectName("Scale");
    d->m_scene->addItem(d->m_CommandWidget);//take ownership

    d->m_widgetItemList.append(d->m_CommandWidget);
}

void IGraphIOAnalog::createEndText()
{
    Q_D(IGraphIOAnalog);

    LabelItemWidget *label = new LabelItemWidget(tr("Output Voltage"));
    label->setObjectName("label_analogIOEndText");
    d->m_endText = new WidgetItem;
    d->m_endText->setWidget(label);
    d->m_endText->setObjectName("EndText");
    d->m_scene->addItem(d->m_endText);

    d->m_widgetItemList.append(d->m_endText);
}

void IGraphIOAnalog::createStartEndItem()
{
    Q_D(IGraphIOAnalog);
    TargetItemWidget *t0 = new TargetItemWidget;
    t0->setObjectName("targetItem_analogioTstart");
    d->m_TStart = new WidgetItem;
    d->m_TStart->setWidget(t0);
    d->m_TStart->setObjectName("TStart");

    t0 = new TargetItemWidget;
    t0->setObjectName("targetItem_analogioTend");
    d->m_TEnd = new WidgetItem;
    d->m_TEnd->setWidget(t0);
    d->m_TEnd->setObjectName("TEnd");

    d->m_scene->addItem(d->m_TStart);
    d->m_scene->addItem(d->m_TEnd);

    d->m_widgetItemList.append(d->m_TStart);
    d->m_widgetItemList.append(d->m_TEnd);
}

void IGraphIOAnalog::createArrowItems()
{
    Q_D(IGraphIOAnalog);
    d->m_A0 = new ArrowItem(d->m_TStart->pointF(WidgetItem::POINT_TYPE_RIGHT), d->m_OfstWidget->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A1 = new ArrowItem(d->m_OfstWidget->pointF(WidgetItem::POINT_TYPE_RIGHT), d->m_ScaleWidget->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A2 = new ArrowItem(d->m_ScaleWidget->pointF(WidgetItem::POINT_TYPE_RIGHT), d->m_CommandWidget->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A3 = new ArrowItem(d->m_CommandWidget->pointF(WidgetItem::POINT_TYPE_RIGHT), d->m_TEnd->pointF(WidgetItem::POINT_TYPE_LEFT));

    d->m_scene->addItem(d->m_A0);
    d->m_scene->addItem(d->m_A1);
    d->m_scene->addItem(d->m_A2);
    d->m_scene->addItem(d->m_A3);

    d->m_arrowList.append(d->m_A0);
    d->m_arrowList.append(d->m_A1);
    d->m_arrowList.append(d->m_A2);
    d->m_arrowList.append(d->m_A3);
}

void IGraphIOAnalog::createAnchorItemHelper()
{
    Q_D(IGraphIOAnalog);
    d->m_anchorHelper = new AnchorItemHelper(this);
}

void IGraphIOAnalog::adjustPosition()
{
    Q_D(IGraphIOAnalog);

    GTUtils::delayms(10);

    if(d->m_anchorHelper!=NULL) {
        d->m_anchorHelper->setAnchorsActive();
    }

    GTUtils::delayms(10);
    foreach (ArrowItem *arrow, d->m_arrowList) {
        arrow->updatePosition();
        arrow->setColor(arrowColor());
    }
}

void IGraphIOAnalog::setUpItemPosAnchors()
{
    Q_D(IGraphIOAnalog);
    qDebug()<<"scene width"<<d->m_scene->width();
    qDebug()<<"scene height"<<d->m_scene->height();
    d->m_OfstWidget->setPos(ofstInitPos());
    d->m_anchorHelper->addAnchor(d->m_OfstWidget, d->m_TStart, AnchorItemHelper::AnchorLeft, -1*d->m_OfstWidget->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_OfstWidget, d->m_TStart, AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_OfstWidget, d->m_ScaleWidget, AnchorItemHelper::AnchorRight, 1.5*d->m_ScaleWidget->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_OfstWidget, d->m_ScaleWidget, AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_ScaleWidget, d->m_CommandWidget, AnchorItemHelper::AnchorRight, 1.5*d->m_OfstWidget->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_ScaleWidget, d->m_CommandWidget, AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_CommandWidget, d->m_TEnd, AnchorItemHelper::AnchorRight, 1.5*d->m_OfstWidget->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_CommandWidget, d->m_TEnd, AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_TStart, d->m_startText, AnchorItemHelper::AnchorLeft, 1.8*d->m_TStart->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_TStart, d->m_startText, AnchorItemHelper::AnchorBottom, -15);

    d->m_anchorHelper->addAnchor(d->m_TEnd, d->m_endText, AnchorItemHelper::AnchorRight, -1.6*d->m_TEnd->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_TEnd, d->m_endText, AnchorItemHelper::AnchorBottom, -15);
}

IGraphIOAnalog::IGraphIOAnalog(IGraphIOAnalogPrivate &dd, QWidget *parent):IGraph(dd, parent) {

}

bool IGraphIOAnalog::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::KeyPress) {
        qDebug()<<"IGraphIOAnalog::eventFilter";
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
            Q_D(IGraphIOAnalog);
            qDebug()<<"enter clicked"<<"object name"<<obj->objectName()<<"syncBoxText2Item";
            QDoubleSpinBox* box = dynamic_cast<QDoubleSpinBox*>(obj);
            d->m_mapping->syncBoxText2Item(box);
            setEditTextStatus(box, OptFace::EDIT_TEXT_STATUS_READY);
            return true;
        }
    }
    return QWidget::eventFilter(obj,event);
}

void IGraphIOAnalog::setDoubleSpinBoxConnections()
{
    Q_D(IGraphIOAnalog);
    connect(d->m_ofstEdit, SIGNAL(editingFinished()), this, SLOT(onDoubleSpinBoxFocusOut()));
    connect(d->m_scaleEdit, SIGNAL(editingFinished()), this, SLOT(onDoubleSpinBoxFocusOut()));
}

void IGraphIOAnalog::installDoubleSpinBoxEventFilter()
{
    Q_D(IGraphIOAnalog);
    d->m_ofstEdit->installEventFilter(this);
    d->m_scaleEdit->installEventFilter(this);
}

