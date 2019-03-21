#ifndef ENCCONFIGMANAGE_H
#define ENCCONFIGMANAGE_H

#include <QObject>
class IEncConfigItem;

class EncConfigManage : public QObject
{
  Q_OBJECT
public:
  explicit EncConfigManage(QObject *parent = 0);
  ~EncConfigManage();
    void creatItemLists(int num);
  void addEncItem(int index, IEncConfigItem *encItem);
  IEncConfigItem *encItem(int typeInx, quint8 index);
  QList<QStringList> itemNames();
  void clearAllEncItem();

  QWidget *curAttributeWidget() const;
  void setCurAttributeWidget(QWidget *w);

signals:

public slots:
private:
  QList<QList<IEncConfigItem*>> m_encItemList;
  QWidget *mp_curAttributeWidget;
};

#endif // ENCCONFIGMANAGE_H
