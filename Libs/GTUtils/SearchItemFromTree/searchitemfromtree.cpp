#include "searchitemfromtree.h"

#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QMessageBox>
#include <QObject>
#include <QStringList>

SearchItemFromTree::SearchItemFromTree()
{

}

SearchItemFromTree::~SearchItemFromTree()
{

}

bool SearchItemFromTree::searchItemByName(const QStringList partNameList, QTreeWidget *treeWidget)
{
    cancelSearchItemByName(treeWidget);
    bool exist = searchItemByNameMethod(partNameList, treeWidget->invisibleRootItem());
    if(!exist){
        cancelSearchItemByName(treeWidget);
    }
    return exist;
}

bool SearchItemFromTree::searchItemByNameMethod(const QStringList partNameList, QTreeWidgetItem *item)
{
    bool exist = false;
    bool hidden = true;
    static int level = 0;
    level++;

    if(item->childCount()==0){
        bool exist_i = false;
        for(int i = 0; i<partNameList.count(); i++){
            exist_i = item->text(0).contains(partNameList.at(i), Qt::CaseInsensitive);
            if(exist_i)
                exist = true;
        }
        if(!exist){
            item->setHidden(true);
        }
    }else{
        for(int i = 0; i<item->childCount(); i++){
            exist = searchItemByNameMethod(partNameList, item->child(i));
            if(exist){
                hidden = false;
            }
        }
        if(hidden){
            item->setHidden(true);
            exist = false;
            item->setExpanded(false);
        }else{
            exist = true;
            item->setExpanded(true);
        }
    }

    level--;
    return exist;
}

bool SearchItemFromTree::cancelSearchItemByName(QTreeWidget *treeWidget)
{
   QTreeWidgetItemIterator it(treeWidget);
   while(*it){
       static_cast<QTreeWidgetItem *>(*it)->setHidden(false);
       ++it;
   }
   return true;
}

