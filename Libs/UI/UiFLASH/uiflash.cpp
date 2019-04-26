#include "uiflash.h"
#include "ui_uiflash.h"
#include "iuiwidget_p.h"
#include "sevdevice.h"
#include "gtutils.h"
#include "bititemhelper.h"
#include <QTreeWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "searchitemfromtree.h"
#include <QLabel>
#include <QStringList>
#include <QTreeWidgetItemIterator>

#define STR_MARK_ALL "all"
#define STR_MARK_BIT "bit"

class UiFLASHPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiFLASH)
public:
  UiFLASHPrivate();
  ~UiFLASHPrivate();
  QTreeWidgetItem *m_editedItem;
  int m_editedCol;
  bool m_isEditing;
  QColor m_nodeColor;
  QString m_originText;

  QLineEdit *m_partNameLineEdit;
protected:
};
UiFLASHPrivate::UiFLASHPrivate()
{

}
UiFLASHPrivate::~UiFLASHPrivate()
{

}

UiFLASH::UiFLASH(QWidget *parent):IUiWidget(*(new UiFLASHPrivate),parent),ui(new Ui::UiFLASH)
{
    Q_D(UiFLASH);
  ui->setupUi(this);
  this->setFocus();
  d->m_editedItem = NULL;
  d->m_isEditing = false;
  d->m_nodeColor = Qt::blue;
  d->m_copyAll = true;
}

UiFLASH::~UiFLASH()
{
    delete ui;
}

bool UiFLASH::writePageFlashToOtherAxis(int srcAxisInx, int desAxisInx, QTreeWidget *tree)
{
    Q_D(UiFLASH);
    Q_UNUSED(srcAxisInx);
    if (!d->m_device->isConnecting() && !d->m_device->isOffline()) {
        return false;
    }
    d->m_msg = tr("Updating data Axis%1").arg(desAxisInx + 1);
    updateTree(d->m_dataTree, tree);
    d->m_barCount = 0;
    d->m_msg = tr("Writing data Axis%1").arg(desAxisInx + 1);
    QTreeWidgetItemIterator it(tree);
    while (*it) {
        if ((*it)->childCount() == 0) {
//            qDebug()<<"item name"<<(*it)->text(0);
//            qDebug()<<"item value"<<(*it)->text(1);
            emit sendBarInfo((d->m_barCount++) % 100, d->m_msg);
            writeItem(desAxisInx, (*it));
        }
        ++it;
    }
    return true;
}

bool UiFLASH::hasConfigFunc()
{
  return false;
}

bool UiFLASH::hasSaveFunc()
{
    return false;
}

QColor UiFLASH::flashNodeColor()
{
    Q_D(UiFLASH);
    return d->m_nodeColor;
}

void UiFLASH::setFlashNodeColor(const QColor &nodeColor)
{
    Q_D(UiFLASH);
    if (d->m_nodeColor != nodeColor) {
        d->m_nodeColor = nodeColor;
        emit flashNodeColorChanged(d->m_nodeColor);
    }
}

void UiFLASH::setContextAction()
{

}

void UiFLASH::addTreeWidget(QTreeWidget *tree)
{
    Q_D(UiFLASH);
    QLabel *hintLabel = new QLabel(this);
    hintLabel->setText(tr("Keyword:"));
    QSpacerItem *harizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    d->m_partNameLineEdit = new QLineEdit(this);
    //d->m_partNameLineEdit->setPlaceholderText(tr("such as:pro;pi"));
    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    QPushButton *executeSearchItemBtn = new QPushButton(this);
    executeSearchItemBtn->setText(tr("Execute Search Item"));
    QPushButton *cancelSearchItemBtn = new QPushButton(this);
    cancelSearchItemBtn->setText(tr("Cancel Search Item"));
    hBoxLayout->addWidget(hintLabel);
    hBoxLayout->addWidget(d->m_partNameLineEdit);
    hBoxLayout->addWidget(executeSearchItemBtn);
    hBoxLayout->addWidget(cancelSearchItemBtn);
    hBoxLayout->addItem(harizontalSpacer);
    d->m_vboxLayout->addLayout(hBoxLayout);
    connect(executeSearchItemBtn, SIGNAL(clicked(bool)), this, SLOT(onExecuteSearchItemBtnClicked()));
    connect(cancelSearchItemBtn, SIGNAL(clicked(bool)), this, SLOT(onCancelSearchItemBtnClicked()));

    d->m_dataTree = tree;
    d->m_vboxLayout->addWidget(tree);
    tree->expandToDepth(1);
    tree->resizeColumnToContents(0);

    d->m_dataTree->installEventFilter(this);
    d->m_partNameLineEdit->installEventFilter(this);
    setItemColor(d->m_dataTree->invisibleRootItem());
    connect(d->m_dataTree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onTreeItemClickedEdit(QTreeWidgetItem*,int)));
    connect(d->m_dataTree, SIGNAL(itemSelectionChanged()), this, SLOT(onActionEditFinished()));
    connect(d->m_dataTree, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
}

bool UiFLASH::eventFilter(QObject *obj, QEvent *event)
{
    Q_D(UiFLASH);
    if(event->type() == QEvent::KeyPress)
    {
        GTUtils::delayms(100);
        if(obj == d->m_dataTree)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            qDebug()<<keyEvent->key();
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                if (d->m_isEditing) {
                    if (d->m_editedItem != NULL) {
                        d->m_editedItem->setFlags(d->m_editedItem->flags()&(~Qt::ItemIsEditable));
                        if (!isEditedDataValid(d->m_editedItem)) {
                            d->m_editedItem->setText(GT::COL_FLASH_RAM_TREE_VALUE, d->m_originText);
                        }
                        if (d->m_device->isConnecting() || d->m_device->isOffline()) {
                            writeItem(uiIndexs().axisInx, d->m_editedItem);
                            d->m_editedItem->setTextColor(d->m_editedCol, Qt::black);
                        } else {
                            QMessageBox::information(this, tr("Warning"), tr("Please connect the device!"));
                        }                       
                        d->m_isEditing = false;
                        d->m_editedItem = NULL;
                    }
                } else {
                    if (d->m_dataTree->selectedItems().length() != 0) {
                        for (int i = 0; i < d->m_dataTree->selectedItems().length(); i++) {
                            QTreeWidgetItem *item = d->m_dataTree->selectedItems().at(i);
                            if (d->m_device->isConnecting() || d->m_device->isOffline()) {
                                writeItem(uiIndexs().axisInx, item);
                                item->setTextColor(GT::COL_FLASH_RAM_TREE_VALUE, Qt::black);
                            } else {
                                QMessageBox::information(this, tr("Warning"), tr("Please connect the device!"));
                            }
                        }
                    }
                }
            return true;
            }
        } else if (obj == d->m_partNameLineEdit) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) {
                onExecuteSearchItemBtnClicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void UiFLASH::updateTree(QTreeWidget *srcTree, QTreeWidget *desTree)
{
    Q_D(UiFLASH);
    d->m_barCount = 0;
    updateTreeItem(srcTree->invisibleRootItem(), desTree->invisibleRootItem());
}

void UiFLASH::updateTreeItem(QTreeWidgetItem *srcTreeItem, QTreeWidgetItem *desTreeItem)
{
    Q_D(UiFLASH);
    for (int i = 0; i < srcTreeItem->childCount(); i++) {
        QString text = srcTreeItem->child(i)->text(GT::COL_PAGE_TREE_NAME);
        for (int j = 0; j < desTreeItem->childCount(); j++) {
            if (text.compare(desTreeItem->child(j)->text(GT::COL_PAGE_TREE_NAME)) == 0) {
                emit sendBarInfo((d->m_barCount++) % 100, d->m_msg);
                readItem(uiIndexs().axisInx, srcTreeItem->child(i));
//                qDebug()<<"src name"<<srcTreeItem->child(i)->text(0);
//                qDebug()<<"des name"<<desTreeItem->child(j)->text(0);
//                qDebug()<<"src value"<<srcTreeItem->child(i)->text(1);
//                qDebug()<<"des value"<<desTreeItem->child(j)->text(1);
                desTreeItem->child(j)->setText(GT::COL_PAGE_TREE_VALUE, srcTreeItem->child(i)->text(GT::COL_PAGE_TREE_VALUE));
                updateTreeItem(srcTreeItem->child(i), desTreeItem->child(j));
//                qDebug()<<"updated des value"<<desTreeItem->child(j)->text(1);
                break;
            }
        }
    }
}

QStackedWidget *UiFLASH::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}
QVBoxLayout *UiFLASH::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiFLASH::setDefaultUi()
{
    setCurrentUiIndex(1);
}

void UiFLASH::setItemColor(QTreeWidgetItem *item)
{
    Q_D(UiFLASH);
    if (item->childCount() != 0) {
        for (int i = 0; i < item->columnCount(); i++) {
            item->setTextColor(i, d->m_nodeColor);
        }
        for (int i = 0; i < item->childCount(); i++) {
            setItemColor(item->child(i));
        }
    }
}

void UiFLASH::updateItemData(QTreeWidgetItem *item)
{
    //Q_D(UiFLASH);
    if (item->text(GT::COL_FLASH_RAM_TREE_ADDR).compare("-1") != 0) {
        readItem(uiIndexs().axisInx, item);
    }
    for (int i = 0; i < item->childCount(); i++) {
        if (item->child(i)->isExpanded() || item->child(i)->childCount() == 0) {
            updateItemData(item->child(i));
        }
    }
}

void UiFLASH::writeItem(int axisInx, QTreeWidgetItem *item)
{
    Q_D(UiFLASH);
    QStringList list = item->text(GT::COL_FLASH_RAM_TREE_NAME).split(".");
    int index = list.length() - 1;
    if (BitItemHelper::isTargetItem(item, STR_MARK_BIT, index)) {
        int index = item->parent()->indexOfChild(item);
        QTreeWidgetItem* allItem = item->parent()->child(index - 1);
        if (allItem->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("Uint64") == 0 || allItem->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("int64") == 0) {
            quint64 value = BitItemHelper::calculate64Bits(item);
            allItem->setText(GT::COL_FLASH_RAM_TREE_VALUE, QString::number(value));
        } else if (allItem->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("Uint32") == 0 || allItem->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("int32") == 0){
            quint32 value = BitItemHelper::calculate32Bits(item);
            allItem->setText(GT::COL_FLASH_RAM_TREE_VALUE, QString::number(value));
        } else {
            quint16 value = BitItemHelper::calculate16Bits(item);
            allItem->setText(GT::COL_FLASH_RAM_TREE_VALUE, QString::number(value));
        }
        if (d->m_device->isOffline()) {
            d->m_device->writeOfflineAdvItem(axisInx, allItem);
            d->m_device->readOfflineAdvItem(axisInx, allItem);
        } else {
            d->m_device->writeAdvFlash(axisInx, allItem);
            d->m_device->readAdvFlash(axisInx, allItem);
        }
    } else {
        if (d->m_device->isOffline()) {
            d->m_device->writeOfflineAdvItem(axisInx, item);
            d->m_device->readOfflineAdvItem(axisInx, item);
        } else {
            d->m_device->writeAdvFlash(axisInx, item);
            d->m_device->readAdvFlash(axisInx, item);
        }
    }
}

void UiFLASH::readItem(int axisInx, QTreeWidgetItem *item)
{
    Q_D(UiFLASH);
    if (d->m_device->isOffline()) {
        d->m_device->readOfflineAdvItem(axisInx, item);
    } else {
        d->m_device->readAdvFlash(axisInx, item);
    }
    QStringList list = item->text(GT::COL_FLASH_RAM_TREE_NAME).split(".");
    int listIndex = list.length() - 1;
    if (BitItemHelper::isTargetItem(item, STR_MARK_ALL, listIndex)) {
        int index = item->parent()->indexOfChild(item);
        QTreeWidgetItem* bitItem = item->parent()->child(index + 1);
        list = bitItem->text(GT::COL_FLASH_RAM_TREE_NAME).split(".");
        listIndex = list.length() - 1;
        if (BitItemHelper::isTargetItem(bitItem, STR_MARK_BIT, listIndex)) {
            if (item->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("Uint64") == 0 || item->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("int64") == 0) {
                quint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toULongLong();
                BitItemHelper::assign64Bits(bitItem, value);
            } else if (item->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("Uint32") == 0 || item->text(GT::COL_FLASH_RAM_TREE_TYPE).compare("int32") == 0) {
                quint32 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toULong();
                BitItemHelper::assign32Bits(bitItem, value);
            } else {
                quint16 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toUShort();
                BitItemHelper::assign16Bits(bitItem, value);
            }
        }
    }
}

void UiFLASH::onExecuteSearchItemBtnClicked()
{
    Q_D(UiFLASH);
    qDebug()<<"ExecuteSearchItemBtn Clicked";
    QStringList partNameList = d->m_partNameLineEdit->text().split(";");
    bool exist = SearchItemFromTree::searchItemByName(partNameList, d->m_dataTree);
    if(!exist){
        for(int i = 0; i<d->m_dataTree->topLevelItemCount(); i++){
            QTreeWidgetItem *item = d->m_dataTree->topLevelItem(i);
            d->m_dataTree->expandItem(item);
            for(int k = 0; k<item->childCount(); k++){
                item->child(k)->setExpanded(true);
            }
        }
        QMessageBox::warning(0, QObject::tr("Warning!"), QObject::tr("No corresponding Item was found!\nPlease input again!"), QMessageBox::Ok);
    }
}

void UiFLASH::onCancelSearchItemBtnClicked()
{
    Q_D(UiFLASH);
    qDebug()<<"CancelSearchItemBtn Clicked";
    SearchItemFromTree::cancelSearchItemByName(d->m_dataTree);
}

void UiFLASH::onTreeItemClickedEdit(QTreeWidgetItem *item, int column)
{
    Q_D(UiFLASH);
    if (column == GT::COL_FLASH_RAM_TREE_VALUE && item->childCount() == 0) {
        item->setFlags(item->flags()|Qt::ItemIsEditable);
        d->m_originText = item->text(GT::COL_FLASH_RAM_TREE_VALUE);
        d->m_dataTree->editItem(item, column);
        d->m_editedItem = item;
        d->m_editedCol = column;
        d->m_isEditing = true;
    } else {
        item->setFlags(item->flags()&(~Qt::ItemIsEditable));
    }
}

void UiFLASH::onActionEditFinished()
{
    Q_D(UiFLASH);
    if (d->m_isEditing) {
        if (d->m_editedItem != NULL) {
            //qDebug()<<"red";
            d->m_editedItem->setFlags(d->m_editedItem->flags()&(~Qt::ItemIsEditable));
            d->m_isEditing = false;
            if (!isEditedDataValid(d->m_editedItem)) {
                d->m_editedItem->setText(GT::COL_FLASH_RAM_TREE_VALUE, d->m_originText);
            } else if (d->m_editedItem->text(GT::COL_FLASH_RAM_TREE_VALUE).compare(d->m_originText) != 0) {
                d->m_editedItem->setTextColor(d->m_editedCol, Qt::red);
            }
            d->m_editedItem = NULL;
        }
    }
}

void UiFLASH::onItemExpanded(QTreeWidgetItem *item)
{
    Q_D(UiFLASH);
    d->m_dataTree->resizeColumnToContents(GT::COL_FLASH_RAM_TREE_NAME);
    if (d->m_device->isConnecting() || d->m_device->isOffline()) {
        updateItemData(item);
    } else {
        qDebug()<<"not connected";
    }
}

bool UiFLASH::isEditedDataValid(QTreeWidgetItem *item)
{
    //Q_D(UiFLASH);
    QString itemType = item->text(GT::COL_FLASH_RAM_TREE_TYPE);
    bool ok;
    QStringList list = item->parent()->text(GT::COL_FLASH_RAM_TREE_NAME).split(".");
    int listIndex = list.length() - 1;
    if (BitItemHelper::isTargetItem(item->parent(), STR_MARK_BIT, listIndex)) {
        //qDebug()<<"1";
        quint64 upLimit = pow(2, item->text(GT::COL_FLASH_RAM_TREE_BITWIDTH).toInt()) - 1;
        //qDebug()<<"up limit"<<upLimit;
        quint64 downLimit = 0;
        //qDebug()<<"down limit"<<downLimit;
        quint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toULongLong(&ok, 10);
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    } else if (itemType.compare("Uint64") == 0) {
        //qDebug()<<"2";
        quint64 upLimit = 18446744073709551615 - 1;
        quint64 downLimit = 0;
        quint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toULongLong(&ok, 10);
        //qDebug()<<"up limit"<<upLimit;
        //qDebug()<<"down limit"<<downLimit;
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    } else if (itemType.compare("int64") == 0) {
        //qDebug()<<"3";
        qint64 upLimit = 9223372036854775807 - 1;
        qint64 downLimit = -9223372036854775807 + 1;
        qint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toLongLong(&ok, 10);
        //qDebug()<<"up limit"<<upLimit;
        //qDebug()<<"down limit"<<downLimit;
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    } else if (itemType.compare("Uint32") == 0) {
        //qDebug()<<"4";
        quint64 upLimit = 4294967295;
        quint64 downLimit = 0;
        quint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toULongLong(&ok, 10);
        //qDebug()<<"up limit"<<upLimit;
        //qDebug()<<"down limit"<<downLimit;
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    } else if (itemType.compare("int32") == 0) {
        //qDebug()<<"5";
        qint64 upLimit = 2147483647;
        qint64 downLimit = -2147483647;
        qint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toLongLong(&ok, 10);
        //qDebug()<<"up limit"<<upLimit;
        //qDebug()<<"down limit"<<downLimit;
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    } else if (itemType.compare("int16") == 0) {
        //qDebug()<<"6";
        qint64 upLimit = 32767;
        qint64 downLimit = -32767;
        qint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toLongLong(&ok, 10);
        //qDebug()<<"up limit"<<upLimit;
        //qDebug()<<"down limit"<<downLimit;
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    } else {
        //qDebug()<<"7";
        quint64 upLimit = 65535;
        quint64 downLimit = 0;
        quint64 value = item->text(GT::COL_FLASH_RAM_TREE_VALUE).toULongLong(&ok, 10);
        //qDebug()<<"up limit"<<upLimit;
        //qDebug()<<"down limit"<<downLimit;
        //qDebug()<<"value"<<value;
        if (value < downLimit || value > upLimit || !ok) {
            //qDebug()<<"false";
            return false;
        }
    }
    return true;
}
