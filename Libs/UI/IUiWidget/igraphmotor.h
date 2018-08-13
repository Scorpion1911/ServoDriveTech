#ifndef IGRAPHMOTOR_H
#define IGRAPHMOTOR_H

#include <QWidget>
#include "igraph.h"
#include "iuiwidget_global.h"

class IGraphMotorPrivate;
class IUiWidget;
class IUIWIDGETSHARED_EXPORT IGraphMotor : public IGraph
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(IGraphMotor)
public:
//  explicit IGraphMotor(QWidget *parent = 0);
  virtual ~IGraphMotor();

protected:
  virtual void setUiVersionName()Q_DECL_OVERRIDE =0;
  virtual void setCustomVisitActive(IUiWidget*uiWidget)Q_DECL_OVERRIDE =0;
  virtual void setupDataMappings()=0;


signals:
  void installMotor();
public slots:
protected slots:
  virtual void onReturnBtnClicked() = 0;
  virtual void onInstallMotorReceived(const QStringList &paraList) = 0;


protected:
  IGraphMotor(IGraphMotorPrivate&dd, QWidget *parent=0);

};

#endif // IGRAPHMOTOR_H
