#ifndef SDTTREEWIDGETNAV_H
#define SDTTREEWIDGETNAV_H

#include <QTreeWidget>

class SdtTreeWidgetNav : public QTreeWidget
{
  Q_OBJECT
public:
  explicit SdtTreeWidgetNav(QWidget *parent = 0);
  void updateSizeHint(QSize & szHint2Set);
  void setAxisNumList(const QList<int> &axisNumList);
  virtual QSize sizeHint() const;

signals:
    void copySingleAxis(int devIndex, int srcAxisIndex, int pageIndex, int desAxisIndex);
    void copyAllAxis(int devIndex, int srcAxisIndex, int pageIndex);
public slots:
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void clearLists();
    int findDevIndex(QTreeWidgetItem *item);
    int findAxisIndex(QTreeWidgetItem *item);
    int findPageIndex(QTreeWidgetItem *item);
    bool isDevItem(QTreeWidgetItem *item);
    bool isAxisItem(QTreeWidgetItem *item);
    bool isPageItem(QTreeWidgetItem *item);
    QString getItemName(QTreeWidgetItem *item);
private slots:
    void onCopySingleAxis();
    void onCopyAllAxis();
protected:
  QSize m_szHint;
  QMenu *m_itemMenu;
  QList<int> m_axisNumList;
  QList<QAction *> m_actionList;
};

#endif // SDTTREEWIDGETNAV_H
