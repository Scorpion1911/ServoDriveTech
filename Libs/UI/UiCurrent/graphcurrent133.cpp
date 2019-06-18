#include "graphcurrent133.h"

#include "igraphcurrent_p.h"
#include "Option"
#include "iuiwidget.h"
#include "boxitemmapping.h"
#include "sevdevice.h"
#include "SDTGraphicsItems/SDTGraphicsItems"
#include "iirfilter.h"
#include "deviceconfig.h"
#include "NodeItems.h"

#include <QDebug>
#include <QTreeWidget>
#include <QDoubleSpinBox>
#include <QRadioButton>

#define PID_POS_X 100
#define PID_POS_Y -100

#define TID_INX 0
#define TIQ_INX 1
#define FND_INX 2
#define FNQ_INX 3
#define FIT_SEL 4
#define PRM_FN1 5
#define PRM_FN2 6
#define PRM_QX2 7
#define PRM_FN_NCH 8
#define PRM_QX_NCH 9
#define PRM_KN_NCH 10
#define CUR_DIR_INDEX 11


class GraphCurrent133Private:public IGraphCurrentPrivate
{
  Q_DECLARE_PUBLIC(GraphCurrent133)
public:
  GraphCurrent133Private(){}
  ~GraphCurrent133Private(){}

protected:
  IIRFilter *m_iirFilter;
  WidgetItem *m_dirItem;
  QRadioButton *m_cwRBtn;
  QRadioButton *m_ccwRBtn;
  ArrowItem *m_A6;

};

GraphCurrent133::GraphCurrent133(QWidget *parent) :
  IGraphCurrent(*(new GraphCurrent133Private),parent)
{
  Q_D(GraphCurrent133);


  d->m_iirFilter = new IIRFilter;
  d->m_iirFilter->setObjectName("current_iirFilter");
  connect(d->m_iirFilter,SIGNAL(configClicked()),this,SLOT(onIIRFilterConfigClicked()));
  connect(d->m_iirFilter,SIGNAL(saveClicked()),this,SLOT(onIIRFilterSaveClicked()));
}

GraphCurrent133::~GraphCurrent133()
{
  Q_D(GraphCurrent133);
  delete d->m_iirFilter;
  qDebug()<<"GraphCurrent133 destruct-->";
}

void GraphCurrent133::onUIFItemClicked()
{
  Q_D(GraphCurrent133);
  QString title = tr("%1 axis = %2 current iir filter")\
      .arg(d->m_dev->deviceConfig()->m_modeName)\
      .arg(d->m_uiWidget->uiIndexs().axisInx + 1);
  d->m_iirFilter->setWindowTitle(title);
  d->m_iirFilter->show();
  int index = d->m_treeWidget->topLevelItem(FIT_SEL)->text(GT::COL_PAGE_TREE_VALUE).toUShort();
  d->m_iirFilter->setFitSelectIndex(index);
  d->m_iirFilter->updatePos();
  qDebug()<<"UIFItem Clicked";

//  NodeBlock *block = new NodeBlock(d->m_UIF);
//  FrameItemWidget *frame = new FrameItemWidget;
//  frame->setMode(1);
//  block->setCenterWidget(frame);
//  block->setPos(-d->m_UIF->boundingRect().width()/2,-block->boundingRect().height());
//  d->m_scene->addItem(block);
}

void GraphCurrent133::setCustomVisitActive(IUiWidget *uiWidget)
{
  Q_UNUSED(uiWidget);
}
void GraphCurrent133::setUiVersionName()
{
  Q_D(GraphCurrent133);
  d->m_versionName="V133";
  setObjectName("GraphCurrent133");
}

void GraphCurrent133::setupDataMappings()
{
  Q_D(GraphCurrent133);

  d->m_mapping->insertBox2MultiItem(d->m_pEdit,d->m_treeWidget->topLevelItem(FND_INX));
  d->m_mapping->insertBox2MultiItem(d->m_pEdit,d->m_treeWidget->topLevelItem(FNQ_INX));
  d->m_mapping->insertBox2MultiItem(d->m_iEdit,d->m_treeWidget->topLevelItem(TID_INX));
  d->m_mapping->insertBox2MultiItem(d->m_iEdit,d->m_treeWidget->topLevelItem(TIQ_INX));

  d->m_mapping->insertBox2Item(d->m_pEdit,d->m_treeWidget->topLevelItem(FNQ_INX));
  d->m_mapping->insertBox2Item(d->m_iEdit,d->m_treeWidget->topLevelItem(TIQ_INX));

  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(FNQ_INX),d->m_pEdit);
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(TIQ_INX),d->m_iEdit);


  d->m_mapping->insertBox2Item(d->m_iirFilter->boxFn1(),d->m_treeWidget->topLevelItem(PRM_FN1));
  d->m_mapping->insertBox2Item(d->m_iirFilter->boxFn2(),d->m_treeWidget->topLevelItem(PRM_FN2));
  d->m_mapping->insertBox2Item(d->m_iirFilter->boxQx2(),d->m_treeWidget->topLevelItem(PRM_QX2));
  d->m_mapping->insertBox2Item(d->m_iirFilter->boxFnNch(),d->m_treeWidget->topLevelItem(PRM_FN_NCH));
  d->m_mapping->insertBox2Item(d->m_iirFilter->boxQxNch(),d->m_treeWidget->topLevelItem(PRM_QX_NCH));
  d->m_mapping->insertBox2Item(d->m_iirFilter->boxKnNch(),d->m_treeWidget->topLevelItem(PRM_KN_NCH));

  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(PRM_FN1),d->m_iirFilter->boxFn1());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(PRM_FN2),d->m_iirFilter->boxFn2());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(PRM_QX2),d->m_iirFilter->boxQx2());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(PRM_FN_NCH),d->m_iirFilter->boxFnNch());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(PRM_QX_NCH),d->m_iirFilter->boxQxNch());
  d->m_mapping->insertItem2Box(d->m_treeWidget->topLevelItem(PRM_KN_NCH),d->m_iirFilter->boxKnNch());
}

void GraphCurrent133::createPidControllerItem()
{
  Q_D(GraphCurrent133);

  QWidget *wpid=new QWidget;
  wpid->setObjectName("widget_currentPid");
  QVBoxLayout *vlayoutTest=new QVBoxLayout(wpid);
  QLabel *title=new QLabel(tr("PID controller"),wpid);
  title->setObjectName("label_currentTitle");
  title->setAlignment(Qt::AlignCenter);
  vlayoutTest->addWidget(title);

  QLabel *pgain=new QLabel(tr("P gain(HZ)"));
  pgain->setObjectName("label_currentPgain");
  vlayoutTest->addWidget(pgain);
  QDoubleSpinBox *pedit=new QDoubleSpinBox(wpid);
  d->m_pEdit=pedit;
  pedit->setObjectName("dspinBox_currentPedit");
  pedit->setMinimum(-32768);
  pedit->setMaximum(32767);
  pedit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  vlayoutTest->addWidget(pedit);

  QLabel *igain=new QLabel(tr("I gain(us)"),wpid);
  igain->setObjectName("label_currentIgain");
  vlayoutTest->addWidget(igain);
  QDoubleSpinBox *iedit=new QDoubleSpinBox(wpid);
  d->m_iEdit=iedit;
  iedit->setObjectName("dspinBox_currentIedit");
  iedit->setMinimum(0);
  iedit->setMaximum(65535);
  iedit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  vlayoutTest->addWidget(iedit);


  wpid->setLayout(vlayoutTest);

  d->m_UPID=new WidgetItem;
  d->m_UPID->setWidget(wpid,true);
  d->m_scene->addItem(d->m_UPID);

  d->m_widgetItemList.append(d->m_UPID);

}

void GraphCurrent133::createDirItem()
{
    Q_D(GraphCurrent133);
    d->m_dirItem = new WidgetItem;
    QWidget *dirWidget = new QWidget;
    dirWidget->setObjectName("widget_curDir");
    QVBoxLayout *vlayout = new QVBoxLayout(dirWidget);
    QLabel *label = new QLabel(tr("DirCtl"),dirWidget);
    label->setAlignment(Qt::AlignCenter);
    label->setObjectName("label_curDirControl");
    QHBoxLayout *hlayout = new QHBoxLayout;
    d->m_cwRBtn = new QRadioButton(dirWidget);
    d->m_cwRBtn->setText(tr("CW"));
    d->m_cwRBtn->setChecked(true);
    d->m_ccwRBtn = new QRadioButton(dirWidget);
    d->m_ccwRBtn->setText(tr("CCW"));
    d->m_ccwRBtn->setChecked(false);
    connect(d->m_cwRBtn,SIGNAL(clicked(bool)),this,SLOT(onRadioBtnClicked(bool)));
    connect(d->m_ccwRBtn,SIGNAL(clicked(bool)),this,SLOT(onRadioBtnClicked(bool)));
    hlayout->addWidget(d->m_cwRBtn);
    hlayout->addWidget(d->m_ccwRBtn);
    vlayout->addWidget(label);
    vlayout->addLayout(hlayout);
    dirWidget->setLayout(vlayout);
    d->m_dirItem->setWidget(dirWidget,true);
    d->m_scene->addItem(d->m_dirItem);
    d->m_widgetItemList.append(d->m_dirItem);
}

void GraphCurrent133::onIIRFilterConfigClicked()
{
  Q_D(GraphCurrent133);
  int index = d->m_iirFilter->currentFitSelectIndex();
  d->m_treeWidget->topLevelItem(FIT_SEL)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(index));
  foreach (QDoubleSpinBox *box, d->m_mapping->boxLists()) {
    d->m_mapping->syncBoxText2Item(box);
  }
  d->m_uiWidget->writeGenPageRAM();
}

void GraphCurrent133::onIIRFilterSaveClicked()
{
  Q_D(GraphCurrent133);
  int index = d->m_iirFilter->currentFitSelectIndex();
  d->m_treeWidget->topLevelItem(FIT_SEL)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(index));
  foreach (QDoubleSpinBox *box, d->m_mapping->boxLists()) {
    d->m_mapping->syncBoxText2Item(box);
  }

  d->m_uiWidget->writePageFLASH();
}

void GraphCurrent133::onRadioBtnClicked(bool checked)
{
    Q_D(GraphCurrent133);
    Q_UNUSED(checked);
    int dir;
    if (d->m_cwRBtn->isChecked()) {
        dir = 0;
    } else {
        dir = 1;
    }
    d->m_treeWidget->topLevelItem(CUR_DIR_INDEX)->setText(GT::COL_PAGE_TREE_VALUE,QString::number(dir));
}

QPointF GraphCurrent133::pidInitPos()
{
    return QPointF(PID_POS_X,PID_POS_Y);
}

void GraphCurrent133::setUpItemPosAnchors()
{
    Q_D(GraphCurrent133);
    d->m_UPID->setPos(pidInitPos());
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USUM,AnchorItemHelper::AnchorLeft,-3*d->m_USUM->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_USUM,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_USUM,d->m_UIF,AnchorItemHelper::AnchorRight,-3*d->m_USUM->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_USUM,d->m_UIF,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UIF,d->m_dirItem,AnchorItemHelper::AnchorLeft,-1.5*d->m_UIF->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UIF,d->m_dirItem,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_dirItem,d->m_Tstart,AnchorItemHelper::AnchorLeft,-1.5*d->m_dirItem->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_dirItem,d->m_Tstart,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UCB,AnchorItemHelper::AnchorHorizontalCenter);
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_UCB,AnchorItemHelper::AnchorBottom,2.2*d->m_UCB->boundingRect().height());

    d->m_anchorHelper->addAnchor(d->m_UCB,d->m_T0,AnchorItemHelper::AnchorRight,1*d->m_UCB->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UCB,d->m_T0,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_Tend,AnchorItemHelper::AnchorRight,1.5*d->m_UPID->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_UPID,d->m_Tend,AnchorItemHelper::AnchorVerticalCenter);

    d->m_anchorHelper->addAnchor(d->m_Tstart,d->m_TextStart,AnchorItemHelper::AnchorLeft,1.2*d->m_Tstart->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_Tstart,d->m_TextStart,AnchorItemHelper::AnchorBottom,-15);

    d->m_anchorHelper->addAnchor(d->m_Tend,d->m_TextEnd,AnchorItemHelper::AnchorRight,-1.2*d->m_Tend->boundingRect().width());
    d->m_anchorHelper->addAnchor(d->m_Tend,d->m_TextEnd,AnchorItemHelper::AnchorBottom,-15);
}

void GraphCurrent133::createArrowItems()
{
    Q_D(GraphCurrent133);
    d->m_A0=new ArrowItem(d->m_Tstart->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_dirItem->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A6=new ArrowItem(d->m_dirItem->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UIF->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A1=new ArrowItem(d->m_UIF->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_USUM->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A2=new ArrowItem(d->m_USUM->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_UPID->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A3=new ArrowItem(d->m_UPID->pointF(WidgetItem::POINT_TYPE_RIGHT),d->m_Tend->pointF(WidgetItem::POINT_TYPE_LEFT));
    d->m_A4=new ArrowItem(d->m_T0->pointF(WidgetItem::POINT_TYPE_LEFT),d->m_UCB->pointF(WidgetItem::POINT_TYPE_RIGHT));
    d->m_A5=new ArrowItem(d->m_UCB->pointF(WidgetItem::POINT_TYPE_LEFT),d->m_USUM->pointF(WidgetItem::POINT_TYPE_BOTTOM),ArrowItem::ARROW_TYPE_CORNER,"-");

    d->m_scene->addItem(d->m_A0);
    d->m_scene->addItem(d->m_A1);
    d->m_scene->addItem(d->m_A2);
    d->m_scene->addItem(d->m_A3);
    d->m_scene->addItem(d->m_A4);
    d->m_scene->addItem(d->m_A5);
    d->m_scene->addItem(d->m_A6);

    d->m_arrowList.append(d->m_A0);
    d->m_arrowList.append(d->m_A1);
    d->m_arrowList.append(d->m_A2);
    d->m_arrowList.append(d->m_A3);
    d->m_arrowList.append(d->m_A4);
    d->m_arrowList.append(d->m_A5);
    d->m_arrowList.append(d->m_A6);
}


