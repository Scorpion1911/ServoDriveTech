#ifndef GRAPHSTATUS136_H
#define GRAPHSTATUS136_H

#include <QWidget>
#include "igraphstatus.h"
#include "uistatus_global.h"

namespace Ui {
class GraphStatus136;
}
class GraphStatus136Private;
class UISTATUSSHARED_EXPORT GraphStatus136 : public IGraphStatus
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(GraphStatus136)
public:
  explicit GraphStatus136(QWidget *parent = 0);
  ~GraphStatus136();
    void updateCurCheckBox() Q_DECL_OVERRIDE;

protected:
  void setUiVersionName() Q_DECL_OVERRIDE;
  void setupDataMappings() Q_DECL_OVERRIDE;
  void setDeviceStatusIconByCss(const QString &css) Q_DECL_OVERRIDE;
  QWidget *alarmBackgroundWidget() Q_DECL_OVERRIDE;
  void addLedErrorToUi() Q_DECL_OVERRIDE;
  quint32 alarmCode() Q_DECL_OVERRIDE;
  bool hasError() Q_DECL_OVERRIDE;
  void updateUiLabelText() Q_DECL_OVERRIDE;


private slots:
  void onBtnClearAlarmClicked();
  void onBtnAlarmHistoryClicked();
  void onCurFoldClicked(bool checked);

private:
  Ui::GraphStatus136 *ui;
};

#endif // GRAPHSTATUS136_H
