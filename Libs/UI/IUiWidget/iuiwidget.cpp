#include "iuiwidget.h"
#include "iuiwidget_p.h"
#include "sevdevice.h"
#include "gtutils.h"

#include "Option"
#include "advusercheck.h"
#include "advusercontainer.h"

#include <QTreeWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QQuickWidget>
#include <QQmlContext>
#include <QAction>
#include <QQmlEngine>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDebug>


//IUiWidget::IUiWidget(QWidget *parent):QWidget(parent),d_ptr(new IUiWidgetPrivate())
//{
//  d_ptr->q_ptr=this;
//}
IUiWidget::~IUiWidget()
{
  delete d_ptr;
//  qDebug()<<"IUiWidget destruct-->";
}
IUiWidget::IUiWidget(IUiWidgetPrivate &dd,QWidget *parent):QWidget(parent),d_ptr(&dd)
{
  d_ptr->q_ptr=this;
}
//特例说明，UiPlot中init(NULL)
//其它的都是传入对应的设备
bool IUiWidget::init(SevDevice *device)
{
  Q_D(IUiWidget);
  d->m_uiStackedWidget=getUiStackedWidget();
  d->m_vboxLayout=getVBoxLayout();
  d->m_device=device;
  if(device !=NULL) {
    connect(device,SIGNAL(dspReset()),this,SLOT(onDspReset())); 
  } else {
      qDebug()<<"dev NULL";
  }

  setDefaultUi();

  return true;
}
void IUiWidget::setCurrentUiIndex(quint8 index)
{
  Q_D(IUiWidget);
  quint8 inx=index;
  if(inx>d->m_uiStackedWidget->count()-1)
    inx=d->m_uiStackedWidget->count()-1;
  d->m_uiStackedWidget->setCurrentIndex(inx);
}
void IUiWidget::createActionSwitchView()
{
  Q_D(IUiWidget);
  d->m_actSwitchView=new QAction(this);
  d->m_actSwitchView->setText(tr("switch to list view"));
  d->m_actSwitchView->setCheckable(true);
  d->m_actSwitchView->setChecked(false);
  connect(d->m_actSwitchView,SIGNAL(triggered(bool)),this,SLOT(onSwitchView(bool)));
  this->addAction(d->m_actSwitchView);
}

void IUiWidget::setContextAction()
{
  Q_D(IUiWidget);
    qDebug()<<"iuiwidget context";
  createActionSwitchView();

  QAction *actSeparator=new QAction(this);
  actSeparator->setSeparator(true);
  this->addAction(actSeparator);

  d->m_actReadRAM=new QAction(this);
  d->m_actReadRAM->setText(tr("RAM value"));
  connect(d->m_actReadRAM,SIGNAL(triggered(bool)),this,SLOT(onActionReadRAM()));
  this->addAction(d->m_actReadRAM);

  d->m_actReadFLASH=new QAction(this);
  d->m_actReadFLASH->setText(tr("FLASH value"));
  connect(d->m_actReadFLASH,SIGNAL(triggered(bool)),this,SLOT(onActionReadFLASH()));
  this->addAction(d->m_actReadFLASH);
}

void IUiWidget::updateTree(QTreeWidget *srcTree, QTreeWidget *desTree)
{
    updateTreeItem(srcTree->invisibleRootItem(), desTree->invisibleRootItem());
}

void IUiWidget::updateTreeItem(QTreeWidgetItem *srcTreeItem, QTreeWidgetItem *desTreeItem)
{
    Q_D(IUiWidget);
    for (int i = 0; i < srcTreeItem->childCount(); i++) {
        QString text = srcTreeItem->child(i)->text(GT::COL_PAGE_TREE_NAME);
        for (int j = 0; j < desTreeItem->childCount(); j++) {
            if (text.compare(desTreeItem->child(j)->text(GT::COL_PAGE_TREE_NAME)) == 0) {
//                qDebug()<<"src name"<<srcTreeItem->child(i)->text(0);
//                qDebug()<<"des name"<<desTreeItem->child(j)->text(0);
//                qDebug()<<"src value"<<srcTreeItem->child(i)->text(1);
//                qDebug()<<"des value"<<desTreeItem->child(j)->text(1);
                emit sendBarInfo((d->m_barCount++) % 100, d->m_msg);
                desTreeItem->child(j)->setText(GT::COL_PAGE_TREE_VALUE, srcTreeItem->child(i)->text(GT::COL_PAGE_TREE_VALUE));
                updateTreeItem(srcTreeItem->child(i), desTreeItem->child(j));
                //qDebug()<<"updated des value"<<desTreeItem->child(j)->text(1);
                break;
            }
        }
    }
}

bool IUiWidget::readOfflinePrm()
{
    Q_D(IUiWidget);
    bool ok = d->m_device->readOffLinePagePrm(d->m_index.axisInx, d->m_dataTree);
    return ok;
}

bool IUiWidget::writeOfflinePrm()
{
    Q_D(IUiWidget);
    bool ok = d->m_device->writeOffLinePagePrm(d->m_index.axisInx, d->m_dataTree);
    return ok;
}

SevDevice*IUiWidget::device()
{
  Q_D(IUiWidget);
  return d->m_device;
}

void IUiWidget::accept(QWidget *w)
{
  Q_UNUSED(w);
}

bool IUiWidget::hasConfigFunc()
{
    Q_D(IUiWidget);
    if (d->m_device->isOffline()) {
        return false;
    }
  return true;
}
bool IUiWidget::hasSaveFunc()
{
    return true;
}

QTreeWidget *IUiWidget::getDataTree()
{
    Q_D(IUiWidget);
    return d->m_dataTree;
}

bool IUiWidget::isCopyAll()
{
    Q_D(IUiWidget);
    return d->m_copyAll;
}

void IUiWidget::addTreeWidget(QTreeWidget *tree)
{
  Q_D(IUiWidget);
  d->m_dataTree=tree;
  d->m_vboxLayout->addWidget(tree);
  tree->resizeColumnToContents(0);
  connect(tree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(onTreeItemClickedEdit(QTreeWidgetItem*,int)));
  setContextAction();
}
void IUiWidget::setUiIndexs(const UiIndexs &indexs)
{
  Q_D(IUiWidget);
  d->m_index=indexs;
}
UiIndexs IUiWidget::uiIndexs() const
{
  Q_D(const IUiWidget);
  return d->m_index;
}
//!
//! \brief IUiWidget::readPageFLASH 委托设备去读FLASH
//!
bool IUiWidget::readPageFLASH()
{
  Q_D(IUiWidget);
  qDebug()<<this->objectName()<<"read flash";
//  emit sglReadPageFlash(d->axisInx,d->m_dataTree);
  bool rOk;
  rOk=d->m_device->readPageFlash(d->m_index.axisInx,d->m_dataTree);
  return rOk;
}

bool IUiWidget::writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)
{
    Q_D(IUiWidget);
    if(d->m_device->isConnecting() == false && d->m_device->isOffline() == false)
      return true;

    bool rOk;
    d->m_barCount = 25;
    d->m_msg = tr("Reading data Axis%1").arg(srcAxisInx);
    if (d->m_device->isOffline()) {
        rOk = d->m_device->readOffLinePagePrm(srcAxisInx, d->m_dataTree);
    } else {
        rOk=d->m_device->readPageFlash(srcAxisInx, d->m_dataTree);
    }

    emit sendBarInfo((d->m_barCount) % 100, d->m_msg);
    if (!rOk) {
        return false;
    }

    d->m_msg = tr("Reading data Axis%1").arg(desAxisInx + 1);
    updateTree(d->m_dataTree, tree);

    bool wOk=true;
    bool needChecked = parametersNeedChecked();
    bool checkOk = true;
    qDebug()<<"needChecked = "<<needChecked;
    if(needChecked) {
        d->m_msg = tr("Checking data Axis%1").arg(srcAxisInx + 1);
        emit sendBarInfo((d->m_barCount += 25) % 100, d->m_msg);
        checkOk = d->m_device->checkPageParameters(desAxisInx, tree);
    }

    if(checkOk) {
        d->m_msg = tr("Writing data Axis%1").arg(srcAxisInx + 1);
        emit sendBarInfo((d->m_barCount += 25) % 100, d->m_msg);
        if (d->m_device->isOffline()) {
            wOk = d->m_device->writeOffLinePagePrm(desAxisInx, tree);
        } else {
            wOk = d->m_device->writePageFlash(desAxisInx, tree);
        }
    }
    else
      wOk = false;

    return wOk;
}

//!
//! \brief IUiWidget::writePageFLASH 委托设备去写FLASH
//!
bool IUiWidget::parametersNeedChecked()
{
  bool needChecked = false;
  OptUser *user = dynamic_cast<OptUser *>(OptContainer::instance()->optItem("optuser"));
  AdvUserCheck *usrCheck = dynamic_cast<AdvUserCheck *>(AdvUserContainer::instance()->advItem("advusercheck"));
  bool isAdmin = user->isAdmin();
  bool isChecked = usrCheck->isChecked();
  if (!isAdmin || (isAdmin && isChecked)) {
      //check
    needChecked = true;
  }
  return needChecked;
}

bool IUiWidget::writePageFLASH()
{
  Q_D(IUiWidget);
  qDebug()<<this->objectName()<<"write page flash";
  if(d->m_device->isConnecting() == false && !d->m_device->isOffline())
    return true;

  bool wOk=true;
  //检查参数
  bool needChecked = parametersNeedChecked();
  bool checkOk = true;
  qDebug()<<"needChecked = "<<needChecked;
  if(needChecked)
    checkOk = d->m_device->checkPageParameters(d->m_index.axisInx,d->m_dataTree);
  if(checkOk) {
      if (d->m_device->isOffline()) {
          wOk = d->m_device->writeOffLinePagePrm(d->m_index.axisInx, d->m_dataTree);
      } else {
          wOk = d->m_device->writePageFlash(d->m_index.axisInx, d->m_dataTree);
//          if (wOk) {
//              QMessageBox::information(0, tr("Tips"), tr("Save success! Please reset DSP!"));
//          }
      }
  }
  else
    wOk = false;

  return wOk;
}

bool IUiWidget::readGenPageRAM()
{
  Q_D(IUiWidget);
  bool wOk=true;
  wOk=d->m_device->readGenPageRAM(d->m_index.axisInx,d->m_dataTree);
  return wOk;
}

bool IUiWidget::writeGenPageRAM()
{
  Q_D(IUiWidget);
  if(d->m_device->isConnecting() == false)
    return true;

  bool wOk=true;
  if(hasConfigFunc())
  {
    bool needChecked = parametersNeedChecked();
    bool checkOk = true;
    if(needChecked)
      checkOk = d->m_device->checkPageParameters(d->m_index.axisInx,d->m_dataTree);

    if(checkOk)
      wOk=d->m_device->writeGenPageRAM(d->m_index.axisInx,d->m_dataTree);
    else
      wOk = false;
  }
  return wOk;
}
void IUiWidget::setUiActive(bool actived)
{
  Q_UNUSED(actived);
//  qDebug()<<"TEST_OUT ui"<<this->objectName()<<"active"<<actived;
}

void IUiWidget::onTreeItemClickedEdit(QTreeWidgetItem *item, int column)
{
  Q_D(IUiWidget);
  if(column==GT::COL_PAGE_TREE_VALUE)
  {
    if(item->childCount()==0)
    {
      item->setFlags(item->flags()|Qt::ItemIsEditable);
      d->m_dataTree->editItem(item,column);
      qDebug()<<"edit "<<item->text(0);
    }
  }
  else item->setFlags(item->flags()&(~Qt::ItemIsEditable));
  qDebug()<<"clicked";
}

void IUiWidget::onSwitchView(bool isList)
{
  Q_D(IUiWidget);
  if(isList)
  {
    d->m_uiStackedWidget->setCurrentIndex(1);
    d->m_actSwitchView->setText(tr("switch to graph view"));
//    onActionReadFuncValueFromRam();
  }
  else
  {
    d->m_uiStackedWidget->setCurrentIndex(0);
    d->m_actSwitchView->setText(tr("switch to list view"));
  }
}

void IUiWidget::onActionReadRAM()
{
  qDebug()<<"read ram";
}

void IUiWidget::onActionReadFLASH()
{
  qDebug()<<"read flash";
}

void IUiWidget::onDspReset()
{

}

void IUiWidget::onOfflineChanged(bool offMode)
{
    Q_UNUSED(offMode);
}
