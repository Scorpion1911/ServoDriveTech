﻿#include "uivelocity.h"
#include "ui_uivelocity.h"
#include "iuiwidget_p.h"
#include "igraphvelocity.h"

#include <QGraphicsScene>
#include <QGraphicsView>

class UiVelocityPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiVelocity)
public:
  UiVelocityPrivate();
  ~UiVelocityPrivate();
protected:
  IGraphVelocity *m_graphVelocityView;
//  QGraphicsScene *m_scene;
};
UiVelocityPrivate::UiVelocityPrivate()
{

}
UiVelocityPrivate::~UiVelocityPrivate()
{

}


UiVelocity::UiVelocity(QWidget *parent):IUiWidget(*(new UiVelocityPrivate),parent),ui(new Ui::UiVelocity)
{
  ui->setupUi(this);

}
UiVelocity::~UiVelocity()
{
  Q_D(UiVelocity);

//  delete d->m_scene;
  delete d->m_graphVelocityView;

  delete ui;
}

void UiVelocity::accept(QWidget *w)
{
  Q_D(UiVelocity);
  ui->qmlHboxLayout->addWidget(w);

//  d->m_scene=new QGraphicsScene;
  d->m_graphVelocityView=dynamic_cast<IGraphVelocity *>(w);
//  d->m_graphVelocityView->setScene(d->m_scene);
}

QStackedWidget *UiVelocity::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}

QVBoxLayout *UiVelocity::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiVelocity::setDefaultUi()
{
  setCurrentUiIndex(0);
}

