#include "optautoload.h"
#include "iopt_p.h"
#include "ui_optautoload.h"

#include <QDebug>
#include <QTimer>
#include <QMessageBox>

class OptAutoLoadPrivate:public IOptPrivate
{
  Q_DECLARE_PUBLIC(OptAutoLoad)
public:
  OptAutoLoadPrivate();
  ~OptAutoLoadPrivate();
  bool m_auto;
  bool m_versionMatch;
};

OptAutoLoadPrivate::OptAutoLoadPrivate()
{

}
OptAutoLoadPrivate::~OptAutoLoadPrivate()
{

}
OptAutoLoad::OptAutoLoad(const QString &optName, QWidget *parent) : IOpt(optName,*new OptAutoLoadPrivate,parent),
  ui(new Ui::OptAutoLoad)
{
  ui->setupUi(this);
  readOpt();

  uiInit();

  connect(ui->checkBox,SIGNAL(clicked(bool)),this,SLOT(onCheckedBoxClicked(bool)));
  connect(ui->checkBoxVersionMatch,SIGNAL(clicked(bool)),this,SLOT(onCheckedBoxVersionMatchClicked()));
}
OptAutoLoad::~OptAutoLoad()
{
  delete ui;
}
void OptAutoLoad::uiInit()
{
  Q_D(OptAutoLoad);
    qDebug()<<"autoload uiInit";
  ui->checkBox->setChecked(d->m_auto);
  ui->checkBoxVersionMatch->setChecked(d->m_versionMatch);
  //ui->checkBoxVersionMatch->setVisible(false);
  if(d->m_auto==true){
      ui->checkBoxVersionMatch->setEnabled(false);
  }
}
bool OptAutoLoad::autoLoad() const
{
  Q_D(const OptAutoLoad);
    return d->m_auto;
}

bool OptAutoLoad::versionMatch() const
{
  Q_D(const OptAutoLoad);
    return d->m_versionMatch;
}

bool OptAutoLoad::optActive()
{
  Q_D(OptAutoLoad);
  d->m_auto=ui->checkBox->isChecked();
  qDebug()<<"opt auto load execute active ";
  emit autoLoadChanged(d->m_auto);
  d->m_versionMatch=ui->checkBoxVersionMatch->isChecked();

//  return false;//test pass
  return true;
}
bool OptAutoLoad::readOpt()
{
  Q_D(OptAutoLoad);
  d->m_auto=data("autoload","auto",false).toBool();
  d->m_versionMatch=data("autoload","versionmatch",false).toBool();
  if(d->m_auto==true){
      d->m_versionMatch=true;
  }

  qDebug()<<"optautoload read opt";

  return true;
}
bool OptAutoLoad::writeOpt()
{
  Q_D(OptAutoLoad);
  saveData("autoload","auto",d->m_auto);
  saveData("autoload","versionmatch",d->m_versionMatch);
  return true;
}
void OptAutoLoad::respondErrorExecute()
{
  QTimer::singleShot(100,this,SLOT(onRespondErrorExe()));
}

void OptAutoLoad::onCheckedBoxClicked(bool checked)
{
  setModify(true);
  if(checked==true){
      ui->checkBoxVersionMatch->setChecked(true);
      ui->checkBoxVersionMatch->setEnabled(false);
  }else{
      ui->checkBoxVersionMatch->setEnabled(true);
  }
  qDebug()<<"check box auto clicked";
}

void OptAutoLoad::onCheckedBoxVersionMatchClicked()
{
  setModify(true);
  qDebug()<<"check box version match clicked";
}

void OptAutoLoad::onRespondErrorExe()
{
  QMessageBox::warning(0,"error","error");
  qDebug()<<"error test";
}

QString OptAutoLoad::nickName() {
    return tr("Autoload");
}
