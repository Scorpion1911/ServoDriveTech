﻿#include "uimotor.h"
#include "ui_uimotor.h"
#include "iuiwidget_p.h"

class UiMotorPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiMotor)
public:
  UiMotorPrivate();
  ~UiMotorPrivate();
protected:

};
UiMotorPrivate::UiMotorPrivate()
{

}
UiMotorPrivate::~UiMotorPrivate()
{

}

UiMotor::UiMotor(QWidget *parent):IUiWidget(*(new UiMotorPrivate),parent),ui(new Ui::UiMotor)
{
  ui->setupUi(this);

}
UiMotor::~UiMotor()
{

}

QStackedWidget *UiMotor::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}
QVBoxLayout *UiMotor::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiMotor::setDefaultUi()
{
  setCurrentUiIndex(1);
}
