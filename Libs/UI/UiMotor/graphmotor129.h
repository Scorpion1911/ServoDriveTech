#ifndef GRAPHMOTOR129_H
#define GRAPHMOTOR129_H

#include <QWidget>
#include "uimotor_global.h"
#include "igraphmotor.h"
#include "optface.h"

namespace Ui {
class GraphMotor129;
}
class GraphMotor129Private;
class QDoubleSpinBox;
class QTreeWidgetItem;
class UIMOTORSHARED_EXPORT GraphMotor129 : public IGraphMotor
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(GraphMotor129)
public:
  explicit GraphMotor129(QWidget *parent = 0);
  ~GraphMotor129();

  void syncTreeDataToUiFace() Q_DECL_OVERRIDE;
signals:
  void installMotor();
protected:
  void setCustomVisitActive(IUiWidget *uiWidget) Q_DECL_OVERRIDE;
  void setUiVersionName() Q_DECL_OVERRIDE;
  void setupDataMappings() Q_DECL_OVERRIDE;

protected slots:
  void onDoubleSpinBoxFocusOut();
  void onMotorInstallationBtnClicked();
  void onReturnBtnClicked() Q_DECL_OVERRIDE;
  void onInstallMotorReceived(const QStringList &paraList) Q_DECL_OVERRIDE;
private:
  Ui::GraphMotor129 *ui;
};

#endif // GRAPHMOTOR129_H
