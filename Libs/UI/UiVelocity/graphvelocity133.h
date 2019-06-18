#ifndef GRAPHVELOCITY133_H
#define GRAPHVELOCITY133_H

#include <QWidget>
#include "uivelocity_global.h"
#include "igraphvelocity.h"

class GraphVelocity133Private;

class UIVELOCITYSHARED_EXPORT GraphVelocity133 : public IGraphVelocity
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(GraphVelocity133)
public:
  explicit GraphVelocity133(QWidget *parent = 0);
  ~GraphVelocity133();

protected:
  void setCustomVisitActive(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
  void setUiVersionName() Q_DECL_OVERRIDE;
  void setupDataMappings() Q_DECL_OVERRIDE;
  void createPidItem() Q_DECL_OVERRIDE;
  void createAutoTuningWidget() Q_DECL_OVERRIDE;
  void createArrowItems() Q_DECL_OVERRIDE;
  QPointF pidInitPos() Q_DECL_OVERRIDE;
  void syncTreeDataToUiFace() Q_DECL_OVERRIDE;
  void installDoubleSpinBoxEventFilter() Q_DECL_OVERRIDE;
  void setDoubleSpinBoxConnections() Q_DECL_OVERRIDE;
  void setUpItemPosAnchors() Q_DECL_OVERRIDE;
  void createVelDirItem() Q_DECL_OVERRIDE;

  void setBtnAutoTurningUiOn(bool on);
  void setAutoTurningProgressBarValue(int v);
  void setProgressBarVisible(bool visible);
protected slots:
  void onAutoTurningProgressValueChanged(int value);
  void onAutoTurningFinish(bool finish);
private slots:
  void onPidComboBoxSWIndexChanged(int index);
  void onBtnAutoStartClicked(bool checked);
  void onRadioBtnClicked(bool checked);
  void onJmRadioBtnClicked(bool checked);

private:
  quint16 readNos(const QString &key);

};

#endif // GRAPHVELOCITY133_H
