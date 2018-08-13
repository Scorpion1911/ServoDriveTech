#include "uiio.h"
#include "ui_uiio.h"
#include "iuiwidget_p.h"
#include "sevdevice.h"
#include "comglobal.h"
#include "deviceconfig.h"
#include "igraphio.h"

#include <QDebug>

class UiIOPrivate:public IUiWidgetPrivate
{
  Q_DECLARE_PUBLIC(UiIO)
public:
  UiIOPrivate();
  ~UiIOPrivate();
protected:
    IGraphIO *m_graphIO;
};
UiIOPrivate::UiIOPrivate():
    m_graphIO(NULL)
{

}
UiIOPrivate::~UiIOPrivate()
{
    qDebug()<<"UiIO Private destruct-->";
}

UiIO::UiIO(QWidget *parent):IUiWidget(*(new UiIOPrivate),parent),ui(new Ui::UiIO)
{
  ui->setupUi(this);

}
UiIO::~UiIO()
{
    Q_D(UiIO);
  delete ui;
    delete d->m_graphIO;
}

void UiIO::accept(QWidget *w)
{
    Q_D(UiIO);
  ui->qmlHboxLayout->addWidget(w);
  d->m_graphIO = dynamic_cast<IGraphIO *>(w);
  d->m_graphIO->visit(this);
  d->m_graphIO->initUi();
}

QStackedWidget *UiIO::getUiStackedWidget(void)
{
  return ui->stackedWidget;
}

QVBoxLayout *UiIO::getVBoxLayout(void)
{
  return ui->verticalLayout_tree;
}
void UiIO::setDefaultUi()
{
    setCurrentUiIndex(0);
}

void UiIO::setContextAction()
{
//    Q_D(UiIO);
//    quint8 comType = d->m_device->deviceConfig()->m_comType;
//    if (comType == 0) {
//        IUiWidget::setContextAction();
//    }
}
