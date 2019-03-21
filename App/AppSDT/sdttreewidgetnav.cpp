#include "sdttreewidgetnav.h"
#include "sdtglobaldef.h"

#include <QMenu>
#include <QDebug>
#include <QContextMenuEvent>

#define STATUS_PAGE_NAME "Status"
#define RAM_PAGE_NAME "RAM"

SdtTreeWidgetNav::SdtTreeWidgetNav(QWidget *parent) : QTreeWidget(parent)
{
    m_szHint = QSize(180,600);
    m_itemMenu = new QMenu(0);
}
void SdtTreeWidgetNav::updateSizeHint(QSize & szHint2Set) {
    m_szHint = szHint2Set;
    updateGeometry();
}

void SdtTreeWidgetNav::setAxisNumList(const QList<int> &axisNumList)
{
    m_axisNumList = axisNumList;
}

QSize SdtTreeWidgetNav::sizeHint() const {
    return m_szHint;
}

void SdtTreeWidgetNav::contextMenuEvent(QContextMenuEvent *event)
{
    m_itemMenu->clear();
    clearLists();
    QPoint point = QCursor::pos();
    QPoint localPos = viewport()->mapFromGlobal(point);
    QTreeWidgetItem *item = itemAt(localPos);
    int devIndex = findDevIndex(item);
    qDebug()<<"devIndex"<<devIndex;
    int axisIndex = findAxisIndex(item);
    qDebug()<<"axisIndex"<<axisIndex;
    int pageIndex = findPageIndex(item);
    qDebug()<<"pageIndex"<<pageIndex;
    QString nickName = getItemName(item);
    if (nickName.compare(STATUS_PAGE_NAME) == 0 || nickName.compare(RAM_PAGE_NAME) == 0) {
        event->accept();
        return;
    }
    if (item != NULL) {
        if (isDevItem(item)) {
            qDebug()<<"dev item";
            event->accept();
            return;
        } else {
            if (isAxisItem(item) && item->childCount() == 0) {
                event->accept();
                return;
            }
            for (int i = 0; i < m_axisNumList.at(devIndex); i++) {
                if (i != axisIndex) {
                    QAction *act = new QAction(tr("Copy to Axis%1").arg(i + 1), 0);
                    QList<QVariant> mapVarList;
                    mapVarList.append(devIndex);
                    mapVarList.append(axisIndex);
                    mapVarList.append(pageIndex);
                    mapVarList.append(i);
                    act->setData(mapVarList);
                    m_actionList.append(act);
                    m_itemMenu->addAction(act);
                    connect(act, SIGNAL(triggered()), this, SLOT(onCopySingleAxis()));
                }
            }
            QAction *copyToAllAxis = new QAction(tr("Copy to all Axis"), 0);
            QList<QVariant> allAxisVarList;
            allAxisVarList.append(devIndex);
            allAxisVarList.append(axisIndex);
            allAxisVarList.append(pageIndex);
            allAxisVarList.append(-1);
            copyToAllAxis->setData(allAxisVarList);
            m_actionList.append(copyToAllAxis);
            connect(copyToAllAxis, SIGNAL(triggered()), this, SLOT(onCopyAllAxis()));
            m_itemMenu->addAction(copyToAllAxis);
        }
        m_itemMenu->exec(QCursor::pos());
    }
    event->accept();
}

void SdtTreeWidgetNav::clearLists()
{
    for (int i = 0; i < m_actionList.count(); i++) {
        delete m_actionList.at(i);
    }
    m_actionList.clear();
}

int SdtTreeWidgetNav::findDevIndex(QTreeWidgetItem *item)
{
    if (item != NULL) {
        for (int i = 0; i < topLevelItemCount(); i++) {
            if (item == topLevelItem(i)) {
                return i;
            }
        }
        return findDevIndex(item->parent());
    }
    return -1;
}

int SdtTreeWidgetNav::findAxisIndex(QTreeWidgetItem *item)
{
    if (item != NULL && item->parent() != NULL) {
        for (int i = 0; i < topLevelItemCount(); i++) {
            if (item->parent() == topLevelItem(i)) {
                return item->parent()->indexOfChild(item);
            }
        }
        return findAxisIndex(item->parent());
    }
    return -1;
}

int SdtTreeWidgetNav::findPageIndex(QTreeWidgetItem *item)
{
    if (item != NULL) {
        if (item->childCount() == 0 && !isDevItem(item) && !isAxisItem(item)) {
            return item->parent()->indexOfChild(item);
        }
    }
    return -1;
}

bool SdtTreeWidgetNav::isDevItem(QTreeWidgetItem *item)
{
    if (item != NULL) {
        for (int i = 0; i < topLevelItemCount(); i++) {
            if (item == topLevelItem(i)) {
                return true;
            }
        }
    }
    return false;
}

bool SdtTreeWidgetNav::isAxisItem(QTreeWidgetItem *item)
{
    if (item != NULL && item->parent() != NULL) {
        for (int i = 0; i < topLevelItemCount(); i++) {
            if (item->parent() == topLevelItem(i)) {
                return true;
            }
        }
    }
    return false;
}

bool SdtTreeWidgetNav::isPageItem(QTreeWidgetItem *item)
{
    if (item != NULL) {
        if (item->childCount() == 0) {
            return true;
        }
    }
    return false;
}

QString SdtTreeWidgetNav::getItemName(QTreeWidgetItem *item)
{
    if (item != NULL) {
        return item->text(GT::COL_TARGET_CONFIG_NICKNAME);
    }
    return "";
}

void SdtTreeWidgetNav::onCopySingleAxis()
{
    QAction *act = static_cast<QAction *>(sender());
    QList<QVariant> varList = act->data().toList();
    int devIndex = varList.at(0).toInt();
    int srcAxisIndex = varList.at(1).toInt();
    int pageIndex = varList.at(2).toInt();
    int desAxisIndex = varList.at(3).toInt();
    emit copySingleAxis(devIndex, srcAxisIndex, pageIndex, desAxisIndex);
}

void SdtTreeWidgetNav::onCopyAllAxis()
{
    QAction *act = static_cast<QAction *>(sender());
    QList<QVariant> varList = act->data().toList();
    int devIndex = varList.at(0).toInt();
    int srcAxisIndex = varList.at(1).toInt();
    int pageIndex = varList.at(2).toInt();
    emit copyAllAxis(devIndex, srcAxisIndex, pageIndex);
}

