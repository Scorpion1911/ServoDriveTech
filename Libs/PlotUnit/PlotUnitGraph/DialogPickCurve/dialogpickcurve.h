#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTreeWidget>

namespace Ui {
class DialogPickCurve;
}
class OptFace;
class QTreeWidget;
class QTreeWidgetItem;
class QTableWidget;
class ICurve;
class SevDevice;

class DialogPickCurve : public QDialog
{
  Q_OBJECT

public:
  explicit DialogPickCurve(SevDevice *sev,QWidget *parent = 0);
  ~DialogPickCurve();

  void expertTreeWidgetInit(const QTreeWidget *tree);
  void axisTableInit(int axisCount);
  void usrCurveTableInit(QList<ICurve *> curves);
  void cusCurveTableInit(QList<QList<ICurve *> > curves);

signals:
  void expertTreeItemDoubleClicked(QTableWidget* axisTable,QTreeWidgetItem *item);
  void addUsrCurveRequest(ICurve *newc);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
  void onExecuteSearchItemBtnClicked();
  void onCancelSearchItemBtnClicked();
  void onUserSelectChanged();
  void onTreeWidgetExpertExpandedClicked();
  void onExpertTreeWidgetDoubleClicked(QTreeWidgetItem *item,int column);
  void onUsrTableCellDoubleClicked(int row, int column);
  void onCusTableCellDoubleClicked(int row, int column);
  void onCusBoxIndexChanged(int index);

private:
  void setIcons();
private:
  Ui::DialogPickCurve *ui;
  OptFace *m_face;
  SevDevice *m_sev;
  QTreeWidget *m_dataTree;
  QLineEdit *m_partNameLineEdit;
  QList<QList<ICurve*>> m_curveList;
};

#endif // DIALOG_H
