#include "dragdropmainwindow.h"
#include "ui_dragdropmainwindow.h"
#include "tree.h"

#include <QLabel>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>

DragDropMainWindow::DragDropMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::DragDropMainWindow)
{
  ui->setupUi(this);
//  setAcceptDrops(false);

//  Tree *tree=new Tree;
//  tree->show();
  QString str1 = "44.000";
  int value1 = str1.toDouble();

  QString str2 = "44";
  quint16 value2 = str2.toUShort();

  qDebug()<<"value1"<<value1;
  qDebug()<<"value2"<<value2;
}

DragDropMainWindow::~DragDropMainWindow()
{
  delete ui;
}

//void DragDropMainWindow::dragEnterEvent(QDragEnterEvent *event)
//{
//  __super::dragEnterEvent(event);
//  if (event->mimeData()->hasUrls())
//      event->acceptProposedAction();
//}

//void DragDropMainWindow::dropEvent(QDropEvent *event)
//{
//  __super::dropEvent(event);
//  if (event->mimeData()->hasUrls())
//  {
//      QList<QUrl> urls = event->mimeData()->urls();
//      foreach (QUrl url , urls)
//      {
////          QLabel *pLabel = new QLabel(this);
////          QImage image(url.toString().mid(8));
////          pLabel->setPixmap(QPixmap::fromImage(image));
////          ui->verticalLayout->addWidget(pLabel);
//        qDebug()<<url.toString();
//      }
//  }
//}
