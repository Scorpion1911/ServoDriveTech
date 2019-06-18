#ifndef USERMODEPANEL_H
#define USERMODEPANEL_H

#include <QWidget>
class QCheckBox;
class QComboBox;

class UiUserCheckCombo:public QWidget
{
  Q_OBJECT
public:
  explicit UiUserCheckCombo(quint16 axis,QWidget *parent);
  ~UiUserCheckCombo();

  QSize sizeHint();
signals:
  void checkChanged(bool checked);
  void modeChanged(int mode);
private:
  friend class UserModePanel;
  quint16 m_axisNum;
  QCheckBox *m_checkBox;
  QComboBox *m_comboBox;
};

class UserModePanel : public QWidget
{
  Q_OBJECT
public:
  explicit UserModePanel(QWidget *parent = 0);
  explicit UserModePanel(quint16 axisCount,QWidget *parent = 0);
  ~UserModePanel();

  void setAxisCount(quint16 axis);
  quint16 axisCount() const;
  bool isChecked(quint16 axis);
  int mode(quint16 axis);
  void setChecked(quint16 axis, bool checked);
  void setMode(quint16 axis,int mode);

protected:
  void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
  void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

signals:
  void checkChanged(quint16 axis,int mode);
  void modeChanged(quint16 axis,int mode);

public slots:
private slots:
  void onCheckChanged(bool checked);
  void onModeChanged(int mode);
private:
  void clearUiCheckComboList();
private:
  bool m_keyCtlIsPressed;
  quint16 m_axisCount;
  QList<UiUserCheckCombo *> m_uiCheckComboList;
};

#endif // USERMODEPANEL_H
