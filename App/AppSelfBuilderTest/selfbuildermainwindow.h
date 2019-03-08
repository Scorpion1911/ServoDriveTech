#ifndef SELFBUILDERMAINWINDOW_H
#define SELFBUILDERMAINWINDOW_H

#include <QMainWindow>
#include "gtutils_global.h"

class QTreeWidget;

namespace ComDriver {
  class ICom;
}

namespace Ui {
class SelfBuilderMainWindow;
}

class SelfBuilderMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SelfBuilderMainWindow(QWidget *parent = 0);
    ~SelfBuilderMainWindow();
private slots:
    void onOpenClicked();
    void onAssignClicked();
    void onSaveClicked();
private:
    Ui::SelfBuilderMainWindow *ui;
    QTreeWidget* m_tree;
};

#endif // SELFBUILDERMAINWINDOW_H
