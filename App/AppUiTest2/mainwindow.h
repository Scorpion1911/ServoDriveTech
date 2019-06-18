#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTreeWidget;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onOpenClicked();
    void onExeClicked();
    void onSaveClicked();
private:
    void changeAddr(QTreeWidgetItem *item);
private:
    Ui::MainWindow *ui;
    QString m_filePath;
    QString m_path;
    QTreeWidget *m_tree;
};

#endif // MAINWINDOW_H
