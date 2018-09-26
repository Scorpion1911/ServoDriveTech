#ifndef SEARCHITEMFROMTREE_H
#define SEARCHITEMFROMTREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QString>
#include <QStringList>
#include "gtutils_global.h"

class GTUTILSSHARED_EXPORT SearchItemFromTree
{
public:
    explicit SearchItemFromTree();
    ~SearchItemFromTree();
    static bool searchItemByName(const QStringList partNameList, QTreeWidget *treeWidget);
    static bool searchItemByNameMethod(const QStringList partNameList, QTreeWidgetItem *item);
    static bool cancelSearchItemByName(QTreeWidget *treeWidget);
};

#endif // SEARCHITEMFROMTREE_H
