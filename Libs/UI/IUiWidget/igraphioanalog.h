#ifndef IGRAPHIOANALOG_H
#define IGRAPHIOANALOG_H

#include <QWidget>
#include "iuiwidget_global.h"
#include "igraph.h"

class IGraphIOAnalogPrivate;
class IUiWidget;

class IUIWIDGETSHARED_EXPORT IGraphIOAnalog : public IGraph
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(IGraphIOAnalog)

  Q_PROPERTY(QColor arrowColor READ arrowColor WRITE setArrowColor NOTIFY arrowColorChanged)
public:
  virtual ~IGraphIOAnalog();

  QColor arrowColor() const;
  void setArrowColor(const QColor &color);
  virtual void setTimerActive(bool en) = 0;
protected:
  virtual void setUiVersionName()Q_DECL_OVERRIDE =0;
  virtual void setCustomVisitActive(IUiWidget*uiWidget)Q_DECL_OVERRIDE =0;
  virtual void setupDataMappings()=0;

signals:
  void arrowColorChanged(const QColor &color);

public slots:
protected:
  IGraphIOAnalog(IGraphIOAnalogPrivate&dd, QWidget *parent=0);
  virtual QPointF ofstInitPos() = 0;
  virtual bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;
  virtual void setDoubleSpinBoxConnections() Q_DECL_OVERRIDE;
  virtual void installDoubleSpinBoxEventFilter() Q_DECL_OVERRIDE;
  virtual void createStartText();
  virtual void createOffsetWidget();
  virtual void createVoltageScale();
  virtual void createVoltageCommand();
  virtual void createEndText();
  virtual void createStartEndItem();

  virtual void createArrowItems();
  virtual void createAnchorItemHelper();
  virtual void adjustPosition();
  virtual void setUpItemPosAnchors();
};


#endif // IGRAPHIOANALOG_H
