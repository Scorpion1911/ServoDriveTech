#ifndef IGRAPHBRAKE_P_H
#define IGRAPHBRAKE_P_H
#include "igraphwidget_p.h"
#include "igraphvelocity.h"
#include "iuiwidget_global.h"

class AnchorItemHelper;
class WidgetItem;
class ArrowItem;
class QDoubleSpinBox;
class QProgressBar;
class QPushButton;
class QRadioButton;
class QLabel;

class IUIWIDGETSHARED_EXPORT IGraphVelocityPrivate:public IGraphWidgetPrivate
{
  Q_DECLARE_PUBLIC(IGraphVelocity)
public:
  IGraphVelocityPrivate(){}
  virtual ~IGraphVelocityPrivate(){}
protected:
  AnchorItemHelper *m_anchorHelper;
  WidgetItem *m_Tstart;
  WidgetItem *m_Tend;
  WidgetItem *m_T0;

  WidgetItem *m_UIF;
  WidgetItem *m_USUM;
  WidgetItem *m_UPID;
  WidgetItem *m_USATN;
  WidgetItem *m_UOF;
  WidgetItem *m_UVB;

  WidgetItem *m_TextStart;
  WidgetItem *m_TextEnd;

  ArrowItem *m_A0;
  ArrowItem *m_A1;
  ArrowItem *m_A2;
  ArrowItem *m_A3;
  ArrowItem *m_A4;
  ArrowItem *m_A5;
  ArrowItem *m_A6;
  ArrowItem *m_A7;

  ArrowItem *m_A13;

  QList<ArrowItem *>m_arrowList;
  QList<WidgetItem *>m_widgetItemList;

  QDoubleSpinBox *m_pEdit;
  QDoubleSpinBox *m_iEdit;

  //饱和输出器
  WidgetItem *m_UFRAME;
  WidgetItem *m_UMAXTQ;
  WidgetItem *m_UMAXTQ_P;
  WidgetItem *m_UMAXTQ_N;

  QDoubleSpinBox *m_maxTqEdit;
  QDoubleSpinBox *m_maxTqEdit_P;
  QDoubleSpinBox *m_maxTqEdit_N;

  WidgetItem *m_T1;
  WidgetItem *m_T2;
  WidgetItem *m_T3;

  ArrowItem *m_A8;
  ArrowItem *m_A9;
  ArrowItem *m_A10;
  ArrowItem *m_A11;
  ArrowItem *m_A12;

  WidgetItem *m_TextMaxTqPositive;
  WidgetItem *m_TextMaxTqNegative;

  WidgetItem *m_autoTnItem;
  QDoubleSpinBox* m_spdBox;
  QProgressBar *m_bar;
  QPushButton *m_btnAutoStart;

  WidgetItem *m_dirItem;
  QRadioButton *m_cwRBtn;
  QRadioButton *m_ccwRBtn;

  QRadioButton *m_jmYesBtn;
  QRadioButton *m_jmNoBtn;

  QLabel *m_jmLabel;
  QLabel *m_jmTextLabel;

  QLabel *m_cycleLable;
  QDoubleSpinBox *m_cycleTimeBox;
};

#endif // IGRAPHBRAKE_P_H

