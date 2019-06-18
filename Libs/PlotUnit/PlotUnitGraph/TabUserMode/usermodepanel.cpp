#include "usermodepanel.h"

#include <QKeyEvent>
#include <QDebug>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QStyledItemDelegate>
#include <QVBoxLayout>

UiUserCheckCombo::UiUserCheckCombo(quint16 axis,QWidget *parent):QWidget(parent),
  m_axisNum(axis)
{
  QHBoxLayout *hlayout=new QHBoxLayout(this);
  hlayout->setMargin(0);
  hlayout->setSpacing(5);
//  QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
  m_checkBox=new QCheckBox;
  m_checkBox->setContentsMargins(0,0,0,0);
  m_checkBox->setObjectName("modeCheckBox");
  m_checkBox->setText(tr("Axis_%1").arg(axis+1));
//  m_checkBox->setSizePolicy(sizePolicy);
  m_comboBox=new QComboBox;
  QStyledItemDelegate* itemDelegate = new QStyledItemDelegate(m_comboBox);
  m_comboBox->setItemDelegate(itemDelegate);
  m_comboBox->setObjectName("modeComboBox");
//  m_comboBox->setSizePolicy(sizePolicy);
  QStringList list;
  list<<tr("0IDLE")\
     <<tr("1PT")\
     <<tr("2VSL")\
     <<tr("3ANAVEL")\
     <<tr("4DIRPILSE");
  m_comboBox->addItems(list);
  hlayout->addWidget(m_checkBox);
  hlayout->addWidget(m_comboBox);
  hlayout->setStretch(0,0);
  hlayout->setStretch(1,1);

  m_comboBox->setCurrentIndex(0);

  connect(m_checkBox,SIGNAL(clicked(bool)),this,SIGNAL(checkChanged(bool)));
  connect(m_comboBox,SIGNAL(currentIndexChanged(int)),this,SIGNAL(modeChanged(int)));
}


UiUserCheckCombo::~UiUserCheckCombo()
{
  delete m_checkBox;
  delete m_comboBox;
}

QSize UiUserCheckCombo::sizeHint()
{
  return QSize(30,100);
}

UserModePanel::UserModePanel(QWidget *parent):QWidget(parent),
  m_keyCtlIsPressed(false),
  m_axisCount(0)
{
  QVBoxLayout *vlayout=new QVBoxLayout(this);
  vlayout->setSpacing(5);
  vlayout->setMargin(0);
  vlayout->setContentsMargins(0,0,0,0);
}

UserModePanel::UserModePanel(quint16 axisCount,QWidget *parent) : QWidget(parent),
  m_axisCount(axisCount),
  m_keyCtlIsPressed(false)
{
  setContentsMargins(0,0,0,0);
  QVBoxLayout *vlayout=new QVBoxLayout(this);
  vlayout->setSpacing(5);
  vlayout->setMargin(0);
  vlayout->setContentsMargins(0,0,0,0);
//  QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

  for(int i=0;i<axisCount;i++)
  {
    UiUserCheckCombo *uiBox=new UiUserCheckCombo(i,this);
    uiBox->setContentsMargins(0,0,0,0);
//    uiBox->setSizePolicy(sizePolicy);
    connect(uiBox,SIGNAL(checkChanged(bool)),this,SLOT(onCheckChanged(bool)));
    connect(uiBox,SIGNAL(modeChanged(int)),this,SLOT(onModeChanged(int)));
    m_uiCheckComboList.append(uiBox);
    vlayout->addWidget(uiBox);
  }
}

UserModePanel::~UserModePanel()
{

}

void UserModePanel::setAxisCount(quint16 axis)
{
  m_axisCount=axis;
  clearUiCheckComboList();
  for(int i=0;i<axis;i++)
  {
    UiUserCheckCombo *uiBox=new UiUserCheckCombo(i,this);
    uiBox->setContentsMargins(0,0,0,0);
    connect(uiBox,SIGNAL(checkChanged(bool)),this,SLOT(onCheckChanged(bool)));
    connect(uiBox,SIGNAL(modeChanged(int)),this,SLOT(onModeChanged(int)));
    m_uiCheckComboList.append(uiBox);
    layout()->addWidget(uiBox);
  }
  m_uiCheckComboList.at(0)->m_checkBox->setChecked(true);
}

bool UserModePanel::isChecked(quint16 axis)
{
  return m_uiCheckComboList.at(axis)->m_checkBox->isChecked();
}

int UserModePanel::mode(quint16 axis)
{
  return m_uiCheckComboList.at(axis)->m_comboBox->currentIndex();
}

void UserModePanel::setChecked(quint16 axis,bool checked)
{
  m_uiCheckComboList.at(axis)->m_checkBox->setChecked(checked);
}

void UserModePanel::setMode(quint16 axis, int mode)
{
    if (mode < m_uiCheckComboList.at(axis)->m_comboBox->count()) {
        m_uiCheckComboList.at(axis)->m_comboBox->setCurrentIndex(mode);
    } else {
        m_uiCheckComboList.at(axis)->m_comboBox->setCurrentIndex(0);
    }
}

void UserModePanel::keyPressEvent(QKeyEvent *event)
{
  Q_UNUSED(event);
  if(event->key()==Qt::Key_Control)
    m_keyCtlIsPressed=true;
}

void UserModePanel::keyReleaseEvent(QKeyEvent *event)
{
  Q_UNUSED(event);
  if(event->key()==Qt::Key_Control)
    m_keyCtlIsPressed=false;
}

void UserModePanel::onCheckChanged(bool checked)
{
  UiUserCheckCombo *uibox=qobject_cast<UiUserCheckCombo *>(sender());
  if(checked)
  {
    qDebug()<<"axis"<<uibox->m_axisNum<<"checked"<<checked;
    if(!m_keyCtlIsPressed)
    {
      for(int i=0;i<m_uiCheckComboList.size();i++)
      {
        if(uibox->m_axisNum==i)
          continue;
        m_uiCheckComboList.at(i)->m_checkBox->setChecked(false);
      }
    }
    emit checkChanged(uibox->m_axisNum,uibox->m_comboBox->currentIndex());
  }
  else
  {
    bool hasChecked=false;
    for(int i=0;i<m_uiCheckComboList.size();i++)
    {
      if(m_uiCheckComboList.at(i)->m_checkBox->isChecked())
        hasChecked=true;
    }
    if(hasChecked==false)
      uibox->m_checkBox->setChecked(true);
  }
}

void UserModePanel::onModeChanged(int mode)
{
  UiUserCheckCombo *uibox=qobject_cast<UiUserCheckCombo *>(sender());
  qDebug()<<"axis"<<uibox->m_axisNum<<"mode"<<mode;
  emit modeChanged(uibox->m_axisNum,uibox->m_comboBox->currentIndex());

  for(int i=0;i<m_uiCheckComboList.size();i++)
  {
    m_uiCheckComboList.at(i)->m_checkBox->setChecked(false);
  }
  uibox->m_checkBox->setChecked(true);
}

void UserModePanel::clearUiCheckComboList()
{
  for(int i=0;i<m_uiCheckComboList.size();i++)
  {
    UiUserCheckCombo *uiBox=m_uiCheckComboList.at(i);
    disconnect(uiBox,SIGNAL(checkChanged(bool)),this,SLOT(onCheckChanged(bool)));
    disconnect(uiBox,SIGNAL(modeChanged(int)),this,SLOT(onModeChanged(int)));
    delete uiBox;
  }
  m_uiCheckComboList.clear();
}

quint16 UserModePanel::axisCount() const
{
  return m_axisCount;
}

