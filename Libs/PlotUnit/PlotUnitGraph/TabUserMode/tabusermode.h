#ifndef TABUSERMODE_H
#define TABUSERMODE_H

#include <QWidget>
#include <QTimer>
#include "itabwidget.h"

namespace Ui {
class TabUserMode;
}
class SevDevice;
class UserModeCtlPrms;

class TabUserMode : public ITabWidget
{
  Q_OBJECT

public:
  explicit TabUserMode(const QString &name, SevDevice *sev, QWidget *parent = 0);
  ~TabUserMode();

  void uiUpdate() Q_DECL_OVERRIDE;
  void resetUi() Q_DECL_OVERRIDE;

protected:
  bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
signals:

private slots:
  void onCssChanged(const QString &css);
  void onModeSpinBoxValueChanged(int value);
  void onModeDoubleSpinBoxValueChanged(double value);
  void onUsrCtlPanelModeChanged(quint16 axis, int mode);
  void onUsrCtlPanelCheckChanged(quint16 axis, int mode);
  void onSaveBtnClicked();
  //void onBtnServoOnClicked(bool checked);
  //void onCheckingAutoTurning();

private:
  void setupIcons(const QString &css);
private:
  Ui::TabUserMode *ui;
  int m_currenAxis;
  QList<UserModeCtlPrms *>m_dataList;
  QTimer m_timer;
  QList<int> m_finishList;

};

#endif // TABUSERMODE_H
